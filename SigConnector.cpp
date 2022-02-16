/*******************************************************************************
SigMixer.cpp Takes a set of signals and sums them together

Copyright(C) 2017  Howard James May

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
1      02-Feb-2017   Initial release
*******************************************************************************/

#include "SigConnector.h"

using namespace SweetMaker;

SigConnector::SigConnector()
{
	output = NULL;
	input = NULL;
}

SigConnector::SigConnector(ISigOutput * _output, ISigInput *_input)
{
	output = _output;
	input = _input;
}

void SigConnector::configPorts(ISigOutput * _output, ISigInput *_input)
{
	output = _output;
	input = _input;
}

void SigConnector::update(uint16_t elapsedTime_ms)
{
	if((input != NULL) && (output != NULL))
	input->writeValue(output->readValue());
}
