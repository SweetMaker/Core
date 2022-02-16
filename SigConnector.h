/*******************************************************************************
SigConnector.h Takes a SigOutput and pushes the value to a SigInput

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

#ifndef __SIG_CONNECTOR_H__
#define __SIG_CONNECTOR_H__

#ifndef NULL
#define NULL (0)
#endif

#include "ISigInput.h"
#include "ISigOutput.h"
#include "Updater.h"

namespace SweetMaker
{
	class SigConnector : private AutoUpdate
	{
	public:
		SigConnector();
		SigConnector(ISigOutput * output, ISigInput * input);
		void configPorts(ISigOutput * output, ISigInput * input);

	protected:
		void update(uint16_t elapsedTime_ms);

	private:
		ISigOutput * output;
		ISigInput * input;
	};
}

#endif