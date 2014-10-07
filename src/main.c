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
#include <stdlib.h>
#include <unistd.h>
#include <jack/jack.h>
#include <alsa/asoundlib.h>

snd_seq_t *open_seq();

int main()
{
	// pointer for a queue;
	int queue;
	// make sure the damn thing will actually run
	printf("Hello world\n");

	// for getting the notes
	snd_seq_event_t *ev;
	int npfd;
	struct pollfd *pfd;
	
	// begin ALSA
	// *handle is passed to many ALSA functions. It is a sequencer handler type
	snd_seq_t *handle;
	
	//open ALSA sequencer
	handle = open_seq();
	
	// set ALSA client name
	snd_seq_set_client_name(handle, "Lights Example");

	// create input port
	snd_seq_create_simple_port(handle, "Port", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_MIDI_GENERIC);

	// create queue. print id to make sure
	queue = snd_seq_alloc_named_queue (handle, "first queue");

	
	npfd = snd_seq_poll_descriptors_count (handle, POLLIN);
	pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
	snd_seq_poll_descriptors (handle, pfd, npfd, POLLIN);

	// run until closed ??
	while (1) {
		if (poll(pfd, npfd, 100000) > 0) {
			do {
				snd_seq_event_input(handle, &ev);
				switch (ev->type) {
					case SND_SEQ_EVENT_CONTROLLER:
						fprintf(stderr, "Control event on channel %2d: %5d    \n",
						        ev->data.control.channel, ev->data.control.value);
						break;
					case SND_SEQ_EVENT_PITCHBEND:
						fprintf(stderr, "Pitchbender event on channel %2d: %5d    \n",
						        ev->data.control.channel, ev->data.control.value);
						break;
					case SND_SEQ_EVENT_NOTEON:
						fprintf(stderr, "NOTE ON event on channel %2d: %5d    \n",
						        ev->data.control.channel, ev->data.note.note);
						break;
					case SND_SEQ_EVENT_NOTEOFF:
						fprintf(stderr, "NOTE OFF event on channel %2d: %5d    \n",
						        ev->data.control.channel, ev->data.note.note);
						break;
				}
				snd_seq_free_event (ev);
			} while (snd_seq_event_input_pending(handle, 0) > 0);
		}
	}
	
	// close sequencer
	if (snd_seq_close(handle)<0) {
		return 1;
	}

	// close queue
	snd_seq_free_queue(handle, queue);
	return (0);
}

snd_seq_t *open_seq() {

  snd_seq_t *seq_handle;
  int portid;

  if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
    fprintf(stderr, "Error opening ALSA sequencer.\n");
    exit(1);
  }
  snd_seq_set_client_name(seq_handle, "ALSA Sequencer Demo");
  if ((portid = snd_seq_create_simple_port(seq_handle, "ALSA Sequencer Demo",
            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "Error creating sequencer port.\n");
    exit(1);
  }
  return(seq_handle);
}
