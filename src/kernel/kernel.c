#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Why when i define "video_memory" as global variable nothing works???

void clear_screen()
{
	short* video_memory = (short*)0xb8000;
		
	for(int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for(int x = 0; x < SCREEN_WIDTH; x++)
		{
			video_memory[y * SCREEN_WIDTH + x] = (0x2f << 8) | ' ';
		}
	}
}

void greeting()
{
	short* video_memory = (short*)0xb8000;

	const char* message = "Welcome to mymoon OS";

	for(const char* c = message; *c != 0; c++, video_memory++)
	{
		*video_memory = (0x2f << 8) | *c;
	}
}

int kmain()
{
	clear_screen();
	greeting();
	return 0;
}