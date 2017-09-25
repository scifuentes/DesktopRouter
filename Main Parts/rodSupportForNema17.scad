$fs=1;

difference() {
	translate(v = [5.0000000000, 0.0000000000, 2.5000000000]) {
		translate(v = [-5.0000000000, -21.1500000000, -2.5000000000]) {
			cube(size = [10, 42.3000000000, 5]);
		}
	}
	translate(v = [0, 0, -0.5000000000]) {
		rotate(a = [0, 90, 0]) {
			cylinder(d = 8, h = 10);
		}
	}
	union() {
		translate(v = [5, 15.5000000000, 0]) {
			translate(v = [0, 0, -6.0000000000]) {
				cylinder(d = 3, h = 12);
			}
		}
		mirror(v = [0, 1, 0]) {
			translate(v = [5, 15.5000000000, 0]) {
				translate(v = [0, 0, -6.0000000000]) {
					cylinder(d = 3, h = 12);
				}
			}
		}
	}
}
/***********************************************
*********      SolidPython code:      **********
************************************************
 
from solid import *
from solid import extensions

class Object:
    pass

nema17 = Object()

rodD=8
screwD=3
nema17.side = 42.3
nema17.holeDistance = 31
thk=1
supportSide=10


rodSupportBase = ( cube([supportSide,nema17.side,rodD/2+1]).align('left','bottom')
                   - cylinder(d=rodD,h=supportSide).rot(y=90).move(z=-.5)
                   - cylinder(d=screwD,h=12).center().move(x=5,y=nema17.holeDistance/2.).mirrorCopy('y'))

scene = rodSupportBase





scad_render_to_file(scene, 'rodSupportForNema17.scad', '$fs=1;')
 
 
************************************************/
