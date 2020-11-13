#!/bin/bash
# export PYTHONPATH=$PYTHONPATH:/usr/lib/python3.8/site-packages
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/lora_sdr/lib64/
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir="/home/martyn/gr-lora-sdr/gr-lora_sdr/python"
export GR_CONF_CONTROLPORT_ON=False
export PATH="/home/martyn/gr-lora-sdr/gr-lora_sdr/build/python":$PATH
export LD_LIBRARY_PATH="":$LD_LIBRARY_PATH
export PYTHONPATH=/home/martyn/gr-lora-sdr/gr-lora_sdr/build/swig:$PYTHONPATH
/usr/bin/python3 /home/martyn/gr-lora-sdr/gr-lora_sdr/apps/sim_lor.py 