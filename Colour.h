/*******************************************************************************
	Colour.h

	Copyright(C) 2015-2016  Howard James May

	This file is part of the SweetMaker SDK

	The SweetMaker SDK is free software: you can redistribute it and / or
	modify it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	The SweetMaker SDK is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see <http://www.gnu.org/licenses/>.

	Contact me at sweet.maker@outlook.com
********************************************************************************
Release     Date                        Change Description
--------|-------------|--------------------------------------------------------|
   1      19-Oct-2016   Initial release
*******************************************************************************/

#ifndef __COLOUR_H__
#define __COLOUR_H__

#include <stdint.h>
#include "ISigInput.h"
#include "ISigOutput.h"

#ifndef NULL
#define NULL (0)
#endif

namespace SweetMaker
{
	typedef enum {
		RED = 0xFF0000L,
		GREEN = 0x00FF00L,
		BLUE = 0x0000FFL,
	}ColourCodes;

	class ColourRGB
	{
	public:
		ColourRGB();
		ColourRGB(uint8_t r, uint8_t g, uint8_t b);
		void setColour(uint8_t r, uint8_t g, uint8_t b);
		void setColour(uint32_t colourCode);
		void operator = (uint32_t colour);

		uint8_t red, green, blue;

	private:
	};

	class ColourHSV
	{
	public:
		ColourHSV();
		ColourHSV(uint8_t h, uint8_t s, uint8_t v);
		void setColour(uint8_t h, uint8_t s, uint8_t v);
		uint8_t hue, saturation, value;
	private:
	};

	class ColourConverter
	{
	public:
		static void ConvertToRGB(ColourHSV * hsv, ColourRGB * rgb);
		static void ConvertToHSV(ColourRGB * rgb, ColourHSV * hsv);
	};

	class HsvToRgbSigConv
	{
	public:
		HsvToRgbSigConv();

		void update();

		void configRedOutput(ISigInput *);
		void configGreenOutput(ISigInput *);
		void configBlueOutput(ISigInput *);

		ISigInput * hueInput;
		ISigInput * saturationInput;
		ISigInput * valueInput;

	private:
		ColourHSV hsv;

		ISigInput * redOutput;
		ISigInput * greenOutput;
		ISigInput * blueOutput;

	};
}

#endif
