%.o: %.c
	@echo + cc $(CURDIR)/$<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	@echo + cc $(CURDIR)/$<
	@$(CC) $(ASFLAGS) -c -o $@ $<