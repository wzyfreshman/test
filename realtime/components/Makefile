CC        = gcc
CFLAGS    = -pipe -O2 -Wall -fPIC

INCLUDES  = -I../../ \
    -I../hal/rtapi \
    -I../hal

LIBS      =

LDFLAGS   =  -ldl

OBJECT_PATH  = 


src=$(wildcard *.comp)
dir=$(notdir $(src))
modules_obj=$(patsubst %.comp,$(OBJECT_PATH)/lib%.so,$(dir) )



all:$(modules_obj)
	@echo $(modules_obj)

%.c:%.comp ./bin/comp
	@echo "Preprocessing $(notdir $<)"
	@mkdir -p $(dir $@)
	$(Q) ./bin/comp $<
	
%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -DRTAPI

$(OBJECT_PATH)/lib%.so:%.o 
	@mkdir -p $(dir $@)
	$(LD) -shared $(LDFLAGS) $< -o $@


clean:
	rm -f $(modules_obj) *.o *.d *.d.* $(TARGETS)

