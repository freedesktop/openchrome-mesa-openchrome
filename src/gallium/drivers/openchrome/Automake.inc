if HAVE_GALLIUM_OPENCHROME

TARGET_DRIVERS += openchrome
TARGET_CPPFLAGS += -DGALLIUM_OPENCHROME
TARGET_LIB_DEPS += \
	$(top_builddir)/src/gallium/winsys/openchrome/drm/libopenchromedrm.la \
	$(top_builddir)/src/gallium/drivers/openchrome/libopenchrome.la \
	$(LIBDRM_LIBS)

endif
