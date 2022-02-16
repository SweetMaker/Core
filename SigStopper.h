/*******************************************************************************
SigStopper.h Takes a signal and inhibits it as requested

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

#ifndef __SIG_STOPPER_H__
#define __SIG_STOPPER_H__

#ifndef NULL
#define NULL (0)
#endif

#include "ISigInput.h"

namespace SweetMaker
{
	class SigStopper :
		public ISigInput
	{
	public:
		SigStopper();
		void configOutput(ISigInput * output);

		void writeValue(int32_t value);

		ISigInput * stopCtrl;

	private:
		ISigInput * output;
		SigInput_bool _stopSignal;
	};
}

#endif