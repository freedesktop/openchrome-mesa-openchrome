#include "state_tracker/drm_driver.h"
#include "target-helpers/inline_debug_helper.h"
#include "openchrome/drm/openchrome_drm_public.h"
#include "openchrome/drm/openchrome_winsys.h"

static struct pipe_screen *
create_screen(int fd)
{
/*   struct radeon_winsys *rw;

	rw = radeon_drm_winsys_create(fd, radeonsi_screen_create);
	return rw ? debug_screen_wrap(rw->screen) : NULL;*/
	return NULL;
}

PUBLIC
DRM_DRIVER_DESCRIPTOR("openchrome", "openchrome", create_screen, NULL)
