/*******************************************************************************
SigStopper.cpp Takes a signal and optionally inhibits it.

Copyright(C) 2016  Howard James May

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
1      05-Dec-2016   Initial release
*******************************************************************************/

#include "SigStopper.h"

using namespace SweetMaker;

SigStopper::SigStopper()
{
	output = NULL;
	_stopSignal.value = false;
	stopCtrl = &_stopSignal;
}

void SigStopper::configOutput(ISigInput *_output)
{
	output = _output;
}

void SigStopper::writeValue(int32_t input)
{
	if (_stopSignal.value == false)
	{
		output->writeValue(input);
	}
	else
	{
		output->writeValue(0);
	}
}