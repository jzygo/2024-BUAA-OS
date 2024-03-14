ifneq ($(test_dir),)
	ifneq ($(test_dir),$(wildcard $(test_dir)))
$(error No such test: $(test_dir) )
	endif

	ifneq ($(wildcard $(test_dir)/Makefile),)
		objects += $(test_dir)/*.x
	endif

	ifneq ($(wildcard $(test_dir)/kernel.mk),)
		include $(test_dir)/kernel.mk
	endif

	ifneq ($(init-override),)
		CFLAGS += -DMOS_INIT_OVERRIDDEN
	else
		ifneq ($(init-envs),)
			CFLAGS += -DMOS_INIT_OVERRIDDEN
		endif
	endif

	ifneq ($(pre-env-run),)
		CFLAGS += -DMOS_PRE_ENV_RUN="$(pre-env-run)"
		ifneq ($(pre-env-run-stmt),)
			CFLAGS += -DMOS_PRE_ENV_RUN_STMT=$(pre-env-run-stmt)
		else
			CFLAGS += -DMOS_PRE_ENV_RUN_STMT="pre_env_run(e);"
		endif
	endif
endif


ifneq ($(wildcard $(test_dir)/Makefile),)
$(test_dir): $(user_modules)
	$(MAKE) --directory=$@

init: $(test_dir)
fs-image: $(test_dir)
endif

include/generated:
	mkdir -p include/generated

.PHONY: all-test init-override init-envs

ifneq ($(init-override),)
init-override: $(test_dir) include/generated
	echo "#include \"$$(realpath $(init-override))\"" > include/generated/init_override.h

init: init-override
endif

ifneq ($(init-envs),)
init-envs: include/generated
	tools/init-gen $(init-envs)

init: init-envs
endif

ifneq ($(pre-env-run),)
pre-env-run: $(test_dir) include/generated
	echo "#include \"$$(realpath $(pre-env-run))\"" > include/generated/pre_env_run.h

init: pre-env-run
endif
