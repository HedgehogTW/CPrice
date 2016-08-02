.PHONY: clean All

All:
	@echo "----------Building project:[ CPrice - Debug ]----------"
	@"$(MAKE)" -f  "CPrice.mk"
clean:
	@echo "----------Cleaning project:[ CPrice - Debug ]----------"
	@"$(MAKE)" -f  "CPrice.mk" clean
