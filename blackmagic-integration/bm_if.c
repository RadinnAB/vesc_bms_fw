/*
	Copyright 2019 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "bm_if.h"
#include "platform.h"
#include "general.h"
#include "target.h"
#include "commands.h"
#include "terminal.h"
#include "target_internal.h"
#include "adiv5.h"

#include <stdio.h>

// Global variables
long cortexm_wait_timeout = 2000; /* Timeout to wait for Cortex to react on halt command. */

// Private variables
static target *cur_target = 0;
static volatile bool target_print_en = true;

// Private functions
static void target_destroy_callback(struct target_controller *tc, target *t) {
	(void)tc;

	if (t == cur_target) {
		cur_target = 0;
	}
}

static void target_printf(struct target_controller *tc, const char *fmt, va_list ap) {
	(void)tc;

	if (target_print_en) {
		char *buf;
		if (vasprintf(&buf, fmt, ap) < 0) {
			return;
		}
		commands_printf(buf);
		free(buf);
	}
}

static struct target_controller gdb_controller = {
	.destroy_callback = target_destroy_callback,
	.printf = target_printf,

	.open = 0,
	.close = 0,
	.read = 0,
	.write = 0,
	.lseek = 0,
	.rename = 0,
	.unlink = 0,
	.stat = 0,
	.fstat = 0,
	.gettimeofday = 0,
	.isatty = 0,
	.system = 0,
};

struct device_info {
	uint16_t designer;
	uint16_t partno;
	int device;
};

/* TODO: Add the following devices:
 * 2: NRF51822 128K/16K
 * 3: NRF51822 256K/16K
 * 4: NRF51822 256K/32K
 * 5: NRF52832 256K/32K
 * 6: NRF52832 256K/64K
 * 7: NRF52832 512K/64K
 */
static const struct device_info device_info_table[] = {
	{.designer = AP_DESIGNER_STM,    .partno = 0x413, .device = 1}, // STM32F40x
	{.designer = AP_DESIGNER_NORDIC, .partno = 0x008, .device = 8}, // NRF52840 1M/256K
};
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static int designer_and_partno_to_device(uint16_t designer, uint16_t partno) {
	for (unsigned int i = 0; i < ARRAY_SIZE(device_info_table); i++) {
		const struct device_info *di = &device_info_table[i];
		if ((di->designer == designer) && (di->partno == partno)) {
			return di->device;
		}
	}

	return -2; // Could not recognize target
}

static int swdp_scan_twice(void) {
	int devs = adiv5_swdp_scan(0);

	if(devs <= 0) {
		devs = adiv5_swdp_scan(0);
	}

	return devs;
}

void bm_init(void) {

}

/**
 * Enable SWD pins as outputs.
 *
 * @param enabled
 * true: SWD pins are used as outputs. Will prevent programmers
 * from accessing this VESC over SWD.
 *
 * false: Use SWD pins as SWD interface. Then this VESC can be
 * programmed over SWD again.
 */
void bm_set_enabled(bool enabled) {
	static bool enabled_now = false;

	if (enabled == enabled_now) {
		return;
	} else {
		enabled_now = enabled;
	}

	if (enabled) {
		SWDIO_MODE_FLOAT();
		palSetPadMode(SWCLK_PORT, SWCLK_PIN, PAL_MODE_OUTPUT_PUSHPULL);
	} else {
		palSetPadMode(SWDIO_PORT, SWDIO_PIN, PAL_MODE_INPUT);
		palSetPadMode(SWCLK_PORT, SWCLK_PIN, PAL_MODE_INPUT);

		// The above does not activate SWD again, so do it explicitly for the SWD pins.
		palSetPadMode(GPIOA, 13, PAL_MODE_ALTERNATE(0));
		palSetPadMode(GPIOA, 14, PAL_MODE_ALTERNATE(0));
	}
}

/**
 * Connect to target and check type.
 *
 * @return
 * -2: Could not recognize target
 * -1: Could not connect
 *  2: Target is NRF51822 128K/16K
 *  3: Target is NRF51822 256K/16K
 *  4: Target is NRF51822 256K/32K
 *  5: Target is NRF52832 256K/32K
 *  6: Target is NRF52832 256K/64K
 *  7: Target is NRF52832 512K/64K
 *  8: Target is NRF52840 1M/256K
 */
int bm_connect(void) {
	int ret = -1;

	bm_set_enabled(true);
	target_print_en = false;

	int devs = swdp_scan_twice();

	if (devs > 0) {
		cur_target = target_attach_n(1, &gdb_controller);

		if (cur_target) {
			uint16_t designer = target_designer(cur_target);
			uint16_t partno = target_idcode(cur_target); // Misleading function name, this is not the idcode
			ret = designer_and_partno_to_device(designer, partno);
			if (ret < 0) {
				commands_printf("Unknown designer/idcode: 0x%04X/0x%04X\n", designer, partno);
			}
		}
	}

	if (ret < 0) {
		bm_disconnect();
	}

	return ret;
}

/**
 * Erase all flash on target.
 *
 * @return
 * -3: Erase failed
 * -2: Could not recognize target
 * -1: Not connected
 *  1: Success
 */
int bm_erase_flash_all(void) {
	int ret = -1;

	if (cur_target) {
		target_print_en = false;

		target_reset(cur_target);
		const char *argv[1];
		argv[0] = "erase_mass";
		ret = target_command(cur_target, 1, argv) ? -3 : 1;
	}

	return ret;
}

/**
 * Write to flash memory on target.
 *
 * @param addr
 * Address to write to
 *
 * @param data
 * The data to write
 *
 * @param len
 * Length of the data
 *
 * @return
 * -2: Write failed
 * -1: Not connected
 *  1: Success
 */
int bm_write_flash(uint32_t addr, const void *data, uint32_t len) {
	int ret = -1;

	if (cur_target) {
		target_print_en = false;
		ret = target_flash_write(cur_target, addr, data, len) ? -2 : 1;
	}

	return ret;
}

/**
 * Read target memory
 *
 * @param addr
 * Address to read from
 *
 * @param data
 * Store the data here
 *
 * @param len
 * Length of the data
 *
 * @return
 * -2: Read failed
 * -1: Not connected
 *  1: Success
 */
int bm_mem_read(uint32_t addr, void *data, uint32_t len) {
	int ret = -1;

	if (cur_target) {
		target_print_en = false;
		target_flash_done(cur_target);
		ret = target_mem_read(cur_target, data, addr, len) ? -2 : 1;
	}

	return ret;
}

/**
 * Reboot target.
 *
 * @return
 * -2: Flash done failed
 * -1: Not connected
 *  1: Success
 */
int bm_reboot(void) {
	int ret = -1;

	if (cur_target) {
		target_print_en = false;
		ret = target_flash_done(cur_target) ? -2 : 1;
		target_reset(cur_target);
	}

	return ret;
}

/**
 * Leave debug mode of NRF5x device. Will reduce the sleep power consumption
 * significantly.
 */
void bm_leave_nrf_debug_mode(void) {
	bm_set_enabled(true);

	if (!target_list) {
		swdp_scan_twice();
	}

	if (target_list) {
		if (strncmp(target_list[0].driver, "Nordic", 6) == 0) {
			adiv5_dp_write(((ADIv5_AP_t**)target_list[0].priv)[0]->dp, ADIV5_DP_CTRLSTAT, 0);
		}
	}

	bm_set_enabled(false);
}

/**
 * Disconnect from target and release SWD bus
 */
void bm_disconnect(void) {
	if (cur_target) {
		target_print_en = false;
		target_flash_done(cur_target); // Ignore for now
		target_detach(cur_target);
		cur_target = 0;
	}

	bm_set_enabled(false);
}

/**
 * Use NRF5x pins
 */
void bm_nrf5x_swd_pins(void) {
	bm_set_enabled(false);
	platform_set_nrf5x_pins();
}

/**
 * Use default SWD pins
 */
void bm_default_swd_pins(void) {
	bm_set_enabled(false);
	platform_set_default_pins();
}
