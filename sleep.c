/*
   Copyright (C) 2002 Kjetil S. Matheussen / Notam

   V0.0.1 - alpha.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ladspa.h>

#define NUM_PORTS 3

struct SleepLadspa{
  LADSPA_Data *ports[NUM_PORTS];
};



static LADSPA_Descriptor SleepDescriptor={0};


const LADSPA_Descriptor *ladspa_descriptor(unsigned long index) {
  if(index>0) return NULL;
  fprintf(stderr,"ladspasleep: ladspa_descriptor");
  return &SleepDescriptor;
}


LADSPA_Handle SleepL_instantiate(
			       const struct _LADSPA_Descriptor * Descriptor,
                               unsigned long                     SampleRate
			       )
{
  struct SleepLadspa *sleepl=calloc(1,sizeof(struct SleepLadspa));
  return sleepl;
}


void SleepL_cleanup(
		  LADSPA_Handle Instance
		  )
{
  struct SleepLadspa *sleepl=(struct SleepLadspa *)Instance;
  free(sleepl);
}



void SleepL_connect_port(
		       LADSPA_Handle Instance,
		       unsigned long Port,
		       LADSPA_Data * DataLocation
		       )
{
  struct SleepLadspa *sleepl=(struct SleepLadspa *)Instance;
  sleepl->ports[Port]=DataLocation;
}

void SleepL_run(
	      LADSPA_Handle Instance,
              unsigned long SampleCount
	      )
{
  unsigned long lokke;
  struct SleepLadspa *sleepl=(struct SleepLadspa *)Instance;

  for(lokke=0;lokke<SampleCount;lokke++){
    sleepl->ports[1][lokke]=sleepl->ports[0][lokke];
  }

  //printf("%d\n",(int)(*(sleepl->ports[2])));
  usleep((int)(*(sleepl->ports[2])));
}


void _init() {
    LADSPA_Descriptor *vd;

    fprintf(stderr,"Initing sleep");

    vd=&SleepDescriptor;

    vd->UniqueID=99999;
    vd->Label=strdup("Sleep");
    

    vd->Name=strdup("Sleeping for n usecs");

    vd->Maker=strdup("Kjetil S. Matheussen");
    vd->Copyright=strdup("<Copyright information not available>");

    vd->PortCount = 3;

    {
      LADSPA_PortRangeHint *hints = calloc(vd->PortCount,sizeof(LADSPA_PortRangeHint));

      hints[2].HintDescriptor=LADSPA_HINT_DEFAULT_0|LADSPA_HINT_BOUNDED_BELOW| LADSPA_HINT_BOUNDED_ABOVE;
      hints[2].LowerBound=0.0f;
      hints[2].UpperBound=1000.0f;
      
      vd->PortRangeHints = hints;
    }

    {
      char **portnames=calloc(vd->PortCount,sizeof(char*));
      
      portnames[0]=strdup("Audio in");
      portnames[1]=strdup("Audio out");
      portnames[2]=strdup("Usecs to sleep");

      vd->PortNames=( const char * const *)portnames;
    }
    
    {
      LADSPA_PortDescriptor *portdescriptors = malloc(vd->PortCount * sizeof(LADSPA_PortDescriptor));
      
      portdescriptors[0]=(const LADSPA_PortDescriptor)(LADSPA_PORT_INPUT|LADSPA_PORT_AUDIO);
      portdescriptors[1]=(const LADSPA_PortDescriptor)(LADSPA_PORT_OUTPUT|LADSPA_PORT_AUDIO);
      portdescriptors[2]=(const LADSPA_PortDescriptor)(LADSPA_PORT_INPUT|LADSPA_PORT_CONTROL);

      vd->PortDescriptors=portdescriptors;
    }
    

    vd->ImplementationData=NULL;
    vd->instantiate=SleepL_instantiate;
    vd->connect_port=SleepL_connect_port;
    vd->run=SleepL_run;
    vd->cleanup=SleepL_cleanup;

    fprintf(stderr,"Inited sleep");
}

void _fini() {
  return;
}
