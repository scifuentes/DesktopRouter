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
