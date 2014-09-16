/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 Eric <eric@Jacklynne>
 * 
 * Lights is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Lights is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <jack/jack.h>
#include <alsa/asoundlib.h>

int main()
{
	// make sure the damn thing will actually run
	printf("Hello world\n");

	// begin ALSA
	// *handle is passed to many ALSA functions. It is a sequencer handler type
	snd_seq_t *handle;
	int err;
	
	//open ALSA sequencer
	err=snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
	if (err<0) {
		return NULL;
	}

	// set ALSA client name
	snd_seq_set_client_name(handle, "Lights");

	// create input port
	snd_seq_create_simple_port(handle, "Port", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_MIDI_GENERIC);

	// run until closed
	while (1) {
#ifdef WIN32
		Sleep(1000);
		
#else
		sleep(1);
#endif
	}

	// close sequencer
	if (snd_seq_close(handle)<0) {
		return 1;
	}
	
	return (0);
}
