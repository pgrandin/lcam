#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wiringPi.h>

const int greenPin = 18;
const int redPin = 23;
const int keyPin = 04;
const int bigredPin = 27;
const int pushPin = 22;

int status = 0;
/* 0 : starting up
   1 : wait for check
   2 : checking
   3 : ready
   4 : done
*/

int
is_change_needed ()
{
  pid_t pid = fork ();
  if (pid == -1)
    {
      // error, no child created
    }
  else if (pid == 0)
    {
      execl ("/usr/bin/python", "/usr/bin/python",
	     "/usr/local/bin/udns-check.py", NULL);
    }
  else
    {
      // parent
      int statval;
      if (waitpid (pid, &statval, 0) == -1)
	{
	  // handle error
	}
      else
	{
	  printf ("%i\n", WEXITSTATUS (statval));
	  return WEXITSTATUS (statval);
	}
    }
  return -1;
}

int
main (void)
{
  wiringPiSetupGpio ();
  pinMode (greenPin, OUTPUT);
  pinMode (redPin, OUTPUT);
  pinMode (keyPin, INPUT);
  pinMode (bigredPin, OUTPUT);
  pinMode (pushPin, INPUT);
  pullUpDnControl (keyPin, PUD_UP);
  pullUpDnControl (pushPin, PUD_UP);
  pwmWrite (redPin, 255);
  pwmWrite (bigredPin, 255);
  pwmWrite (greenPin, 255);
  digitalWrite (bigredPin, LOW);

  while (1)
    {

      if (status == 5)
	{
	  digitalWrite (bigredPin, HIGH);
	  delay (150);
	  digitalWrite (bigredPin, HIGH);
	  delay (150);
	}
      if (status == 3)
	{
	  digitalWrite (bigredPin, HIGH);
	}
      else
	{
	  digitalWrite (bigredPin, LOW);
	}
      if (status == 4)
	{
	  digitalWrite (greenPin, HIGH);
	}
      else
	{
	  digitalWrite (greenPin, LOW);
	}
      if (digitalRead (pushPin))
	{
	  // push button is released
	}
      else
	{
	  if (status == 3)
	    {
	      status = 4;
	      digitalWrite (bigredPin, LOW);


	      pid_t pid = fork ();
	      if (pid == -1)
		{
		  // error, no child created
		}
	      else if (pid == 0)
		{
		  execl ("/usr/bin/python", "/usr/bin/python",
			 "/usr/local/bin/udns.py", NULL);
		}
	      else
		{
		  // parent
		  int statval;
		  if (waitpid (pid, &statval, 0) == -1)
		    {
		      // handle error
		    }
		  else
		    {
		      printf ("%i\n", WEXITSTATUS (statval));
		      if (WEXITSTATUS (statval) == 1)
			{
			  printf ("success\n");
			  status = 4;
			}
		      else
			{
			  printf ("failure\n");
			  status = 5;
			}
		    }
		}

	    }
	}
      if (digitalRead (keyPin))
	{
	  digitalWrite (redPin, HIGH);
	  delay (75);
	  digitalWrite (redPin, LOW);
	  delay (75);
	  digitalWrite (redPin, LOW);
	  status = 1;
	}
      else
	{
	  digitalWrite (redPin, HIGH);
	  if (status < 3)
	    {
	      if (is_change_needed () == 1)
		{
		  status = 3;
		}
	      else
		{
		  status = 5;
		}
	    }
	}
    }

  return 0;
}
