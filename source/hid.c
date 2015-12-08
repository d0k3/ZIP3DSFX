#include <3ds.h>

/* loop until key is pressed */
u32 wait_key (void) {
	hidScanInput();
	u32 old_kDown, kDown;
	old_kDown = hidKeysDown();

	while (aptMainLoop()) {
		gspWaitForVBlank();

		hidScanInput();
		kDown = hidKeysDown();
		if (kDown != old_kDown)
			break;

		gfxFlushBuffers();
		gfxSwapBuffers();
	}
	return kDown;
}
