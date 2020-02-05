%.dis: %.riscv
	$(RISCV_OBJDUMP) -M numeric --disassemble-all -S $< > $@

%.S: %.c
	$(RISCV_GCC) $(RISCV_GCC_OPTS) $(RISCV_DEFINES) $(RISCV_INCLUDES) -S -fverbose-asm $< -o $@ 

%.S: %.cpp $(BSG_MACHINE_PATH)/Makefile.machine.include
	$(RISCV_GXX) $(RISCV_GXX_OPTS) $(RISCV_DEFINES) $(RISCV_INCLUDES) -S -fverbose-asm $< -o $@

stats: vanilla_stats.csv
	python3 $(BSG_MANYCORE_DIR)/software/py/vanilla_stats_parser.py --tile --tile_group

graphs: blood_abstract.png blood_detailed.png
%/graphs: %/blood_abstract.png %/blood_detailed.png ;

blood_detailed.png: vanilla_operation_trace.csv vanilla_stats.csv
	python3 $(BSG_MANYCORE_DIR)/software/py/blood_graph.py --input vanilla_operation_trace.csv --timing-stats vanilla_stats.csv --generate-key

blood_abstract.png: vanilla_operation_trace.csv vanilla_stats.csv
	python3 $(BSG_MANYCORE_DIR)/software/py/blood_graph.py --input vanilla_operation_trace.csv --timing-stats vanilla_stats.csv --generate-key --abstract

%/blood_detailed.png: %/vanilla_operation_trace.csv %/vanilla_stats.csv
	cd $(dir $<) &&  python3 $(BSG_MANYCORE_DIR)/software/py/blood_graph.py --input vanilla_operation_trace.csv --timing-stats vanilla_stats.csv --generate-key

%/blood_abstract.png: %/vanilla_operation_trace.csv %/vanilla_stats.csv
	cd $(dir $<) &&  python3 $(BSG_MANYCORE_DIR)/software/py/blood_graph.py --input vanilla_operation_trace.csv --timing-stats vanilla_stats.csv --generate-key --abstract

%/stats: %/vanilla_stats.csv
	cd $(dir $<) && python3 $(BSG_MANYCORE_DIR)/software/py/vanilla_stats_parser.py --tile --tile_group

analysis.clean:
	rm -rf *.dis
	rm -rf vanilla_stats.csv vanilla_operation_trace.csv vanilla.log vcache_non_blocking_stats.log vcache_blocking_stats.log
	rm -rf stats
	rm -rf blood_abstract.png blood_detailed.png
	rm -rf key_abstract.png key_detailed.png

.PRECIOUS: %.png %/blood_detailed.png %/blood_abstract.png
