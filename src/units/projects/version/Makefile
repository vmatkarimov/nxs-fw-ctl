include config.mk

PROJ_UNITS_SUB_HEADERS		= $(shell find ./src/units/projects/version/ -mindepth 2 -maxdepth 2 -type f -name "*.h")

objs/nxs-fw-ctl-u-projects-version.o: ./src/units/projects/version/version.c \
					./src/units/projects/version/version.h \
					./src/units/projects/version/version-typedefs.h \
					./src/units/projects/version/version-subunits.h \
					$(PROJ_CORE_HEADERS) \
					$(PROJ_META_HEADERS) \
					$(PROJ_UNITS_SUB_HEADERS) \
					$(PROJ_COLLECTIONS_HEADERS) \
					$(PROJ_DAL_HEADERS) \
					$(NXS_CORE_OBJS) \
					$(NXS_CORE_HEADERS)

					$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-u-projects-version.o ./src/units/projects/version/version.c
