include scripts/Makefile

modules_make = $(MAKE) -C $(1);
modules_clean = $(MAKE) clean -C $(1);
.PHONY: all mm mc clean
all:$(TARGET)
mm:
	@ $(foreach n,$(MODULES),$(call modules_make,$(n)))
mc:
	@ $(foreach n,$(MODULES),$(call modules_clean,$(n)))

$(TARGET) : mm
	$(CC) $(CFLAGS) -o $(TARGET) $(ALLOBJS) $(LIBS) $(INCLUDES)
	@ echo $(TARGET) make done!

clean : mc
	rm -rf $(TARGET)
	@ echo clean done!
