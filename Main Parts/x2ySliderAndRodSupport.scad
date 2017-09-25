$fs=1;

union() {
	difference() {
		rotate(a = [0, 90, 0]) {
			difference() {
				translate(v = [0, 0, -10.0000000000]) {
					cylinder(d = 15, h = 20);
				}
				translate(v = [0, 0, -10.0000000000]) {
					cylinder(d = 12, h = 20);
				}
			}
		}
		#translate(v = [0, -4, 0]) {
			translate(v = [0, -10.0000000000, 0]) {
				translate(v = [-10.0000000000, -10.0000000000, -10.0000000000]) {
					cube(size = 20);
				}
			}
		}
	}
	translate(v = [0, 8, 0]) {
		rotate(a = [-90, 0, 0]) {
			difference() {
				cylinder(d = 11, h = 10);
				cylinder(d = 8, h = 10);
			}
		}
	}
}
/***********************************************
*********      SolidPython code:      **********
************************************************
 
from solid import *
from solid import extensions

sliderL=20
sliderD=12
sliderHold = (cylinder(d=sliderD+3,h=sliderL).center()-cylinder(d=sliderD,h=sliderL).center()).rot(y=90)-cube(sliderL).align('front').move(y=-sliderD/3).debug()

rodD=8
rodL=10
rodSupport = (cylinder(d=rodD+3,h=rodL)-cylinder(d=rodD,h=rodL)).rot(x=-90).move(y=sliderD/2+2)

scene = sliderHold + rodSupport
scad_render_to_file(scene, 'x2ySliderAndRodSupport.scad', '$fs=1;')
 
 
************************************************/
