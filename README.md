#SchmittTrigger Library#

SchmittTrigger implements a Nysteresis dual threshold action. This particular kind of trigger is useful for determining digital states in noisy environments. A Schmitt trigger is a comparator application which switches the output to true when the input passes through a press reference (press_threshold) - The Output is switched back to false when the input passes through a release reference (release_threshold). The SchmittTrigger object supports debouncing of the underlying reference signal.

Copyright (c) 2018-2019 Jerome Drouin  All rights reserved.

SchmittTrigger was originally written by Jerome Drouin and is now maintained by Jerome Drouin.

![SchmittTrigger Demo](http://www.pjrc.com/teensy/td_libs_SchmittTrigger_1.jpg)
