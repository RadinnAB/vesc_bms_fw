BLACKMAGICSRC =	blackmagic/swdptap.c \
				blackmagic/timing.c \
				blackmagic/target/swdptap_generic.c \
				blackmagic/target/target.c \
				blackmagic/target/adiv5_swdp.c \
				blackmagic/target/adiv5.c \
				blackmagic/target/cortexm.c \
				blackmagic/target/nrf51.c \
				blackmagic-integration/bm_if.c \
				blackmagic-integration/exception.c \
				blackmagic-integration/platform.c \

BLACKMAGICINC =	blackmagic \
				blackmagic/target \
				blackmagic-integration \

# Shared variables
ALLCSRC += $(BLACKMAGICSRC)
ALLINC  += $(BLACKMAGICINC)
				