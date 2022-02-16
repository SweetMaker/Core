/*******************************************************************************
    Colour.cpp

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

#include "Colour.h"

using namespace SweetMaker;

ColourRGB::ColourRGB()
{
	red = 0;
	green = 0;
	blue = 0;
}

ColourRGB::ColourRGB(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

void ColourRGB::operator = (uint32_t colour) 
{ 
	blue = colour & 0xff;
	colour = colour >> 8;
	green = colour & 0xff;
	colour = colour >> 8;
	red = colour & 0xff;
}


void ColourRGB::setColour(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

ColourHSV::ColourHSV()
{
	hue = 0;
	saturation = 0;
	value = 0;
}

ColourHSV::ColourHSV(uint8_t h, uint8_t s, uint8_t v)
{
	hue = h;
	saturation = s;
	value = v;
}

void ColourHSV::setColour(uint8_t h, uint8_t s, uint8_t v)
{
	hue = h;
	saturation = s;
	value = v;
}

void ColourConverter::ConvertToRGB(ColourHSV * hsv, ColourRGB * rgb)
{
    uint8_t sector;
    uint16_t x, y, z, sectorProgress;

    if (hsv->saturation == 0)
    {
        rgb->red = hsv->value;
        rgb->green = hsv->value;
        rgb->blue = hsv->value;
        return;
    }

    if(hsv->hue >= 128)
    {
		if (hsv->hue >= 213)
		{
			sector = 5;
			sectorProgress = (hsv->hue - 213) * 6;
		}
		else if (hsv->hue >= 170)
		{
			sector = 4;
			sectorProgress = (hsv->hue - 170) * 6;
		}
		else
		{
			sector = 3;
			sectorProgress = (hsv->hue - 128) * 6;
		}
    }
	else
	{
		if (hsv->hue >= 85)
		{
			sector = 2;
			sectorProgress = (hsv->hue - 85) * 6;
		}
		else if (hsv->hue >= 43)
		{
			sector = 1;
			sectorProgress = (hsv->hue - 43) * 6;
		}
		else
		{
			sector = 0;
			sectorProgress = hsv->hue * 6;
		}
    }

    x = (hsv->value * (255 - hsv->saturation)) >> 8;
    y = (hsv->value * (255 - ((hsv->saturation * sectorProgress) >> 8))) >> 8;
    z = (hsv->value * (255 - ((hsv->saturation * (255 - sectorProgress)) >> 8))) >> 8;

    switch (sector)
    {
        case 0:
            rgb->red = hsv->value; 
            rgb->green = z; 
            rgb->blue = x;
            break;
        case 1:
            rgb->red = y; 
            rgb->green = hsv->value; 
            rgb->blue = x;
            break;
        case 2:
            rgb->red = x; 
            rgb->green = hsv->value; 
            rgb->blue = z;
            break;
        case 3:
            rgb->red = x; 
            rgb->green = y; 
            rgb->blue = hsv->value;
            break;
        case 4:
            rgb->red = z; 
            rgb->green = x; 
            rgb->blue = hsv->value;
            break;
        case 5:
            rgb->red = hsv->value; 
            rgb->green = x; 
            rgb->blue = y;
            break;
    }

	return;
}

void ColourConverter::ConvertToHSV(ColourRGB * rgb, ColourHSV * hsv)
{
    uint8_t rgbMin, rgbMax;
    uint8_t chroma;

    rgbMin = rgb->red < rgb->green ? (rgb->red < rgb->blue ? rgb->red : rgb->blue) : (rgb->green < rgb->blue ? rgb->green : rgb->blue);
    rgbMax = rgb->red > rgb->green ? (rgb->red > rgb->blue ? rgb->red : rgb->blue) : (rgb->green > rgb->blue ? rgb->green : rgb->blue);
    
    chroma = rgbMax - rgbMin;

    hsv->value = rgbMax;
    if (hsv->value == 0)
    {
        hsv->hue = 0;
        hsv->saturation = 0;
        return;
    }

    hsv->saturation = 255 * long(chroma) / hsv->value;

    if (hsv->saturation == 0)
    {
        hsv->hue = 0;
    }
    else if (rgbMax == rgb->red)
    {
        hsv->hue = 0 + 43 * (rgb->green - rgb->blue) / (chroma);
    }
    else if (rgbMax == rgb->green)
    {
        hsv->hue = 85 + 43 * (rgb->blue - rgb->red) / (chroma);
    }
    else
    {
        hsv->hue = 171 + 43 * (rgb->red - rgb->green) / (chroma);
    }

    return;
}

HsvToRgbSigConv::HsvToRgbSigConv()
{
	hsv.hue = 0;
	hsv.saturation = 0;
	hsv.value = 0;

	hueInput = new SigInput_u8p(&hsv.hue);
	saturationInput = new SigInput_u8p(&hsv.saturation);
	valueInput = new SigInput_u8p(&hsv.value);
}

void HsvToRgbSigConv::configRedOutput(ISigInput * input)
{
	redOutput = input;
}

void HsvToRgbSigConv::configGreenOutput(ISigInput * input)
{
	greenOutput = input;
}

void HsvToRgbSigConv::configBlueOutput(ISigInput * input)
{
	blueOutput = input;
}

void HsvToRgbSigConv::update()
{
	ColourRGB rgb;

	ColourConverter::ConvertToRGB(&hsv, &rgb);

	if (redOutput != NULL)
		redOutput->writeValue((int32_t)rgb.red);
	if (greenOutput != NULL)
		greenOutput->writeValue((int32_t)rgb.green);
	if (blueOutput != NULL)
		blueOutput->writeValue((int32_t)rgb.blue);

}
