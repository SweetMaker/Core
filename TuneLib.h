/*******************************************************************************
TuneLib.h Provides Tunes for use with PiezoDriver

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
   1      24-Feb-2016   Initial release
*******************************************************************************/

#ifndef __TUNE_LIB_H__
#define __TUNE_LIB_H__

namespace SweetMaker {
#define IMPERIAL_MARCH_NOTE_COUNT (46)
	NOTE imperialMarchNotes[IMPERIAL_MARCH_NOTE_COUNT] = {
		{ NOTE_G4_US, 8 },
		{ NOTE_G4_US, 8 },
		{ NOTE_G4_US, 8 },
		{ NOTE_DS4_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_G4_US, 8 },
		{ NOTE_DS4_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_G4_US, 16 },

		{ NOTE_D5_US, 8 },
		{ NOTE_D5_US, 8 },
		{ NOTE_D5_US, 8 },
		{ NOTE_DS5_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_FS4_US, 8 },
		{ NOTE_DS4_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_G4_US, 16 },

		{ NOTE_G5_US, 8 },
		{ NOTE_G4_US, 6 },
		{ NOTE_G4_US, 2 },
		{ NOTE_G5_US, 8 },
		{ NOTE_FS5_US, 6 },
		{ NOTE_F5_US, 2 },
		{ NOTE_E5_US, 2 },
		{ NOTE_DS5_US, 2 },
		{ NOTE_E5_US, 2 },
		{ NOTE_REST_US, 6 },

		{ NOTE_G4_US, 2 },
		{ NOTE_REST_US, 2 },

		{ NOTE_CS5_US, 8 },
		{ NOTE_C5_US, 6 },
		{ NOTE_B4_US, 2 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_A4_US, 2 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_REST_US, 6 },
		{ NOTE_DS4_US, 2 },
		{ NOTE_REST_US, 2 },
		{ NOTE_FS4_US, 9 },

		{ NOTE_DS4_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_G4_US, 8 },
		{ NOTE_DS4_US, 6 },
		{ NOTE_AS4_US, 2 },
		{ NOTE_G4_US, 16 }
	};
#define IMPERIAL_MARCH_TEMPO (1000)

#define JINGLE_BELLS_NOTE_COUNT (51)
	NOTE jingleBellsNotes[JINGLE_BELLS_NOTE_COUNT] = {
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, MINIM },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, MINIM },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_G4_US, CROTCHET },
		{ NOTE_C4_US, CROTCHET },
		{ NOTE_D4_US, CROTCHET },

		{ NOTE_E4_US, SEMIBREVE },

		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },

		{ NOTE_F4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, QUAVER },
		{ NOTE_E4_US, QUAVER },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_D4_US, CROTCHET },
		{ NOTE_D4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },

		{ NOTE_D4_US, MINIM },
		{ NOTE_G4_US, MINIM },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, MINIM },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, MINIM },

		{ NOTE_E4_US, CROTCHET },
		{ NOTE_G4_US, CROTCHET },
		{ NOTE_C4_US, CROTCHET },
		{ NOTE_D4_US, CROTCHET },

		{ NOTE_E4_US, SEMIBREVE },

		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },

		{ NOTE_F4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, CROTCHET },
		{ NOTE_E4_US, QUAVER },
		{ NOTE_E4_US, QUAVER },

		{ NOTE_G4_US, CROTCHET },
		{ NOTE_G4_US, CROTCHET },
		{ NOTE_F4_US, CROTCHET },
		{ NOTE_D4_US, CROTCHET },

		{ NOTE_C4_US, SEMIBREVE }
	};

#define JINGLE_BELLS_TEMPO (600)

#define STAR_WARS_NOTE_COUNT (89)
	NOTE starWarsNotes[] = {
		{ NOTE_D4_US, 4 },
		{ NOTE_D4_US, 4 },

		{ NOTE_D4_US, 4 },
		{ NOTE_G4_US, 24 },
		{ NOTE_D5_US, 24 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_G5_US, 24 },
		{ NOTE_D5_US, 12 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_G5_US, 24 },
		{ NOTE_D5_US, 12 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_C5_US, 4 },
		{ NOTE_A4_US, 24 },
		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 },

		{ NOTE_G4_US, 24 },
		{ NOTE_D5_US, 24 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_G5_US, 24 },
		{ NOTE_D5_US, 12 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_G5_US, 24 },
		{ NOTE_D5_US, 12 },
		{ NOTE_C5_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_C5_US, 4 },
		{ NOTE_A4_US, 24 },

		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 },

		{ NOTE_E4_US, 18 },
		{ NOTE_E4_US, 6 },
		{ NOTE_C5_US, 3 },
		{ NOTE_B4_US, 3 },
		{ NOTE_A4_US, 3 },
		{ NOTE_G4_US, 3 },
		{ NOTE_G4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 12 },
		{ NOTE_E4_US, 6 },
		{ NOTE_FS4_US, 12 },
		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 },

		{ NOTE_E4_US, 18 },
		{ NOTE_E4_US, 6 },
		{ NOTE_C5_US, 3 },
		{ NOTE_B4_US, 3 },
		{ NOTE_A4_US, 3 },
		{ NOTE_G4_US, 3 },

		{ NOTE_D5_US, 3 },
		{ NOTE_A4_US, 3 },
		{ NOTE_A4_US, 24 },
		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 },

		{ NOTE_E4_US, 18 },
		{ NOTE_E4_US, 6 },
		{ NOTE_C5_US, 3 },
		{ NOTE_B4_US, 3 },
		{ NOTE_A4_US, 3 },
		{ NOTE_G4_US, 3 },

		{ NOTE_G4_US, 4 },
		{ NOTE_A4_US, 4 },
		{ NOTE_B4_US, 4 },
		{ NOTE_A4_US, 12 },
		{ NOTE_E4_US, 6 },
		{ NOTE_FS4_US, 12 },
		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 },

		{ NOTE_G5_US, 3 },
		{ NOTE_F5_US, 3 },
		{ NOTE_DS5_US, 3 },
		{ NOTE_D5_US, 3 },
		{ NOTE_C5_US, 3 },
		{ NOTE_AS4_US, 3 },
		{ NOTE_A4_US, 3 },
		{ NOTE_G4_US, 3 },

		{ NOTE_D5_US, 36 },
		{ NOTE_D4_US, 12 },
		{ NOTE_D4_US, 6 }
	};

#define STARWARS_TEMPO (2400)


	NOTE closeEncounters[5] = {
		{ NOTE_D6_US, 3 },
		{ NOTE_E6_US, 4 },
		{ NOTE_C6_US, 5 },
		{ NOTE_C5_US, 4 },
		{ NOTE_G5_US, 9 }
	};
	#define CLOSEENCOUNTERS_TEMPO (400)

}

#endif

