#ifndef TEST_BUILD

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include "shell/command-line-parser.h"
#include "imaging/bitmap.h"
#include "imaging/bmp-format.h"
#include "imaging/bmp-format.h"
#include "midi/midi.h"
using namespace midi;
using namespace std;
using namespace shell;
using namespace imaging;

void draw_rectangle(Bitmap& bitmap, const Position& pos,
	const uint32_t& width, const uint32_t& height,
	const Color& color, const Color& rand)
{
	for (uint32_t i = 0; i < width; i++)
	{
		for (uint32_t j = 0; j < height; j++)
		{
			if (i < 1 || j < 1|| i > width - 2 || j > height - 2) 
			{
				bitmap[Position(pos.x + i, pos.y + j)] = rand;
			}
			else
			{
				bitmap[Position(pos.x + i, pos.y + j)] = color;
			}
		}
	}
}

int getWidth(vector<NOTE> notes)
{
	int res = 0;
	for (NOTE n : notes)
	{
		if (value(n.start + n.duration) > res)
		{
			res = value(n.start + n.duration);
		}
	}
	cout << "Width with scale 1 =========== " << res << endl;
	return res;
}
int getLowestNote(vector<NOTE> notes)
{
	int res = 127;
	for (NOTE n : notes)
	{
		if (value(n.note_number) < res)
		{
			res = value(n.note_number);
		}
	}
	cout << "Lowest note found ====== " << res << endl;
	return res;
}
int getHighestNote(vector<NOTE> notes)
{
	int res = 0;
	for (NOTE n : notes)
	{
		if (value(n.note_number) > res)
		{
			res = value(n.note_number);
		}
	}
	cout << "highest note found ====== " << res << endl;
	return res;
}

int main(int argn, char* argv[])
{
	// TEST
	Bitmap bm(500, 500);
	draw_rectangle(bm, Position(200, 200), 100, 30,
		Color(1, 0, 0), Color(1,1,1));
	save_as_bmp("C:\\tmp\\bitmaptester.bmp", bm);
	// TEST

	string file = "C:\\tmp\\midiFiles\\trololo.mid";
	string outfile = "C:\\tmp\\frames\\frame%d.bmp";
	uint32_t height = 16;
	uint32_t scale = 10;
	uint32_t step = 1;
	uint32_t framewidth = 0;
	
	// Nu lezen uit commmandline ofzoiets
	CommandLineParser parser;
	parser.add_argument(string("-w"), &framewidth);
	parser.add_argument(string("-d"), &step);
	parser.add_argument(string("-s"), &scale);
	parser.add_argument(string("-h"), &height);
	parser.process(vector<string>(argv + 1, argv + argn));
	vector<string> arrgs = parser.positional_arguments();
	if (arrgs.size() >= 1)
	{
		file = arrgs[0];
	}
	if (arrgs.size() >= 2)
	{
		outfile = arrgs[1];
	}

	ifstream in(file, ifstream::binary);
	vector<NOTE> notes = read_notes(in);
	uint32_t mapwidth = getWidth(notes) / scale;
	if (framewidth == 0)
	{
		framewidth = mapwidth;
	}
	int low = getLowestNote(notes);
	int high = getHighestNote(notes);
	cout << "Actual bitmap size =========== " << mapwidth << " x " 
		<< (high - low + 1) * height << endl;
	Bitmap bitmap(mapwidth , 127 * height);
	for (int i = 0; i <= 127; i++) {
		for (NOTE n : notes) {
			if (n.note_number == NoteNumber(i)) {
				draw_rectangle(bitmap, Position(value(n.start) / scale, 
					(127 - i) * height), 
					value(n.duration) / scale, height, 
					Color(0, 0, 1), Color(1,1,1));
			}
		}
	}
	cout << "All notes have been succesfully read" << endl;
	// zwarte randen knippen
	bitmap = *bitmap.slice(0, height * (127 - high),
		mapwidth, (high - low + 1) * height).get();
	// opslaan

	for (int i = 0; i <= mapwidth - framewidth; i += step)
	{
		Bitmap temp = *bitmap.slice(i, 0, framewidth,
			(high - low + 1) * height).get();

		stringstream nummerken;
		nummerken << setfill('0') << setw(5) << (i / step);
		string out = outfile;
		save_as_bmp(out.replace(out.find("%d"),
			2, nummerken.str()), temp);

		cout << "Frame: " << (i / step) << " created" << endl;
	}
	return 0;
}
#endif
