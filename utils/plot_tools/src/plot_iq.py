#!/usr/bin/env python
#
# Copyright 2007,2008,2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
#

import numpy

try:
    from pylab import *
except ImportError:
    print("Please install Python Matplotlib (http://matplotlib.sourceforge.net/) and \
           Python TkInter https://wiki.python.org/moin/TkInter to run this script")
    raise SystemExit(1)

from argparse import ArgumentParser

class plot_iq(object):
    def __init__(self, filename, options):
        self.hfile = open(filename, "r")
        self.block_length = options.block
        self.start = options.start
        self.sample_rate = options.sample_rate

        self.datatype = numpy.complex64
        self.sizeof_data = self.datatype().nbytes    # number of bytes per sample in file

        self.axis_font_size = 16
        self.label_font_size = 18
        self.title_font_size = 20
        self.text_size = 22

        # Setup PLOT
        self.fig = figure(1, figsize=(16, 9), facecolor='w')
        rcParams['xtick.labelsize'] = self.axis_font_size
        rcParams['ytick.labelsize'] = self.axis_font_size

        self.text_file     = figtext(0.10, 0.94, ("File: %s" % filename), weight="heavy", size=self.text_size)
        self.text_file_pos = figtext(0.10, 0.88, "File Position: ", weight="heavy", size=self.text_size)
        self.text_block    = figtext(0.40, 0.88, ("Block Size: %d" % self.block_length),
                                     weight="heavy", size=self.text_size)
        self.text_sr       = figtext(0.60, 0.88, ("Sample Rate: %.2f" % self.sample_rate),
                                     weight="heavy", size=self.text_size)
        self.make_plots()

        self.button_left_axes = self.fig.add_axes([0.45, 0.01, 0.05, 0.05], frameon=True)
        self.button_left = Button(self.button_left_axes, "<")
        self.button_left_callback = self.button_left.on_clicked(self.button_left_click)

        self.button_right_axes = self.fig.add_axes([0.50, 0.01, 0.05, 0.05], frameon=True)
        self.button_right = Button(self.button_right_axes, ">")
        self.button_right_callback = self.button_right.on_clicked(self.button_right_click)

        self.xlim = self.sp_iq.get_xlim()

        self.manager = get_current_fig_manager()
        connect('key_press_event', self.click)
        show()

    def get_data(self):
        self.text_file_pos.set_text("File Position: %d" % (self.hfile.tell()//self.sizeof_data))
        try:
            self.iq = numpy.fromfile(self.hfile, dtype=self.datatype, count=self.block_length)
        except MemoryError:
            print("End of File")
        else:
            self.reals = numpy.array([r.real for r in self.iq])
            self.imags = numpy.array([i.imag for i in self.iq])
            self.time = numpy.array([i*(1/self.sample_rate) for i in range(len(self.reals))])

    def make_plots(self):
        # if specified on the command-line, set file pointer
        self.hfile.seek(self.sizeof_data*self.start, 1)

        self.get_data()

        # Subplot for real and imaginary parts of signal
        self.sp_iq = self.fig.add_subplot(2,1,1, position=[0.075, 0.14, 0.85, 0.67])
        self.sp_iq.set_title(("I&Q"), fontsize=self.title_font_size, fontweight="bold")
        self.sp_iq.set_xlabel("Time (s)", fontsize=self.label_font_size, fontweight="bold")
        self.sp_iq.set_ylabel("Amplitude (V)", fontsize=self.label_font_size, fontweight="bold")
        self.plot_iq = plot(self.time, self.reals, 'bo-', self.time, self.imags, 'ro-')
        self.sp_iq.set_ylim([1.5*min([self.reals.min(), self.imags.min()]),
                             1.5*max([self.reals.max(), self.imags.max()])])
        self.sp_iq.set_xlim(self.time.min(), self.time.max())
        draw()

    def update_plots(self):
        self.plot_iq[0].set_data([self.time, self.reals])
        self.plot_iq[1].set_data([self.time, self.imags])
        self.sp_iq.set_ylim([1.5*min([self.reals.min(), self.imags.min()]),
                             1.5*max([self.reals.max(), self.imags.max()])])
        self.sp_iq.set_xlim(self.time.min(), self.time.max())
        draw()

    def click(self, event):
        forward_valid_keys = [" ", "down", "right"]
        backward_valid_keys = ["up", "left"]

        if(find(event.key, forward_valid_keys)):
            self.step_forward()

        elif(find(event.key, backward_valid_keys)):
            self.step_backward()

    def button_left_click(self, event):
        self.step_backward()

    def button_right_click(self, event):
        self.step_forward()

    def step_forward(self):
        self.get_data()
        self.update_plots()

    def step_backward(self):
        # Step back in file position
        if(self.hfile.tell() >= 2*self.sizeof_data*self.block_length ):
            self.hfile.seek(-2*self.sizeof_data*self.block_length, 1)
        else:
            self.hfile.seek(-self.hfile.tell(),1)
        self.get_data()
        self.update_plots()


def find(item_in, list_search):
    try:
        return list_search.index(item_in) != None
    except ValueError:
        return False

def main():
    description = "Takes a GNU Radio complex binary file and displays the I&Q data versus time. You can set the block size to specify how many points to read in at a time and the start position in the file. By default, the system assumes a sample rate of 1, so in time, each sample is plotted versus the sample number. To set a true time axis, set the sample rate (-R or --sample-rate) to the sample rate used when capturing the samples."

    parser = ArgumentParser(conflict_handler="resolve", description=description)
    parser.add_argument("-B", "--block", type=int, default=1000,
            help="Specify the block size [default=%(default)r]")
    parser.add_argument("-s", "--start", type=int, default=0,
            help="Specify where to start in the file [default=%(default)r]")
    parser.add_argument("-R", "--sample-rate", type=float, default=1.0,
            help="Set the sampler rate of the data [default=%(default)r]")
    parser.add_argument("file", metavar="FILE",
            help="Input file with complex samples")

    args = parser.parse_args()

    dc = draw_iq(args.file, args)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
