/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2017 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/


#pragma once

#include "Types.h"
#include "../Blip_Buffer/blip_buffer.h"

class CMixerChannelBase {
public:
	explicit CMixerChannelBase(unsigned maxVol);

	void SetVolume(double vol);
	void SetLowPass(const blip_eq_t &eq);

private:
	template <typename> friend class CMixerChannel;
	Blip_Synth<blip_good_quality> synth_;
	double lastSum_ = 0.;
};

template <typename T>
class CMixerChannel : public CMixerChannelBase {
public:
	using CMixerChannelBase::CMixerChannelBase;

	void AddValue(chan_id_t ChanID, int Value, int FrameCycles, Blip_Buffer &bb) {
		levels_.UpdateLevel(ChanID, Value);
		const double prev = lastSum_;
		lastSum_ = levels_.CalcPin();
		const double Delta = lastSum_ - prev;
		synth_.offset(FrameCycles, static_cast<int>(Delta), &bb);
	}

	void ResetDelta() {
		lastSum_ = 0;
		levels_ = T { };
	}

private:
	T levels_;
};
