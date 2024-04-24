init-envs  := loop/1 loop/2
CFLAGS     += -DMOS_SCHED_MAX_TICKS=30
pre-env-run := $(test_dir)/pre_env_run.c
