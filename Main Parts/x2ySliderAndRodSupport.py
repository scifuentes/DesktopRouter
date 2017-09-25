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
