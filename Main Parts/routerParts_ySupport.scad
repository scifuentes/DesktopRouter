$fs=.75;

difference(){
	difference() {
		union() {
			difference() {
				rotate(a = [0, 90, 0]) {
					translate(v = [-0.0000000000, -0.0000000000, -42.0000000000]) {
						cylinder(d = 19.0000000000, h = 84.0000000000);
					}
				}
			}
			union() {
				translate(v = [30.0000000000, 0, 0]) {
					intersection() {
						difference() {
							hull() {
								union() {
									rotate(a = [0, 90, 0]) {
										translate(v = [-0.0000000000, -0.0000000000, -12.0000000000]) {
											cylinder(d = 19.0000000000, h = 24.0000000000);
										}
									}
									translate(v = [0, 9.5000000000, 0]) {
										translate(v = [0.0000000000, 5.0000000000, 0.0000000000]) {
											translate(v = [-8.5000000000, -5.0000000000, -8.5000000000]) {
												cube(size = [17.0000000000, 10, 17.0000000000]);
											}
										}
									}
								}
							}
							translate(v = [0, 9.5000000000, 0]) {
								rotate(a = [-90, 0, 0]) {
									cylinder(d = 8.0000000000, h = 10);
								}
							}
							union() {
								translate(v = [6.5000000000, 12, 0]) {
									translate(v = [-0.0000000000, -0.0000000000, -9.5000000000]) {
										cylinder(d = 3.0000000000, h = 19.0000000000);
									}
								}
								mirror(v = [1, 0, 0]) {
									translate(v = [6.5000000000, 12, 0]) {
										translate(v = [-0.0000000000, -0.0000000000, -9.5000000000]) {
											cylinder(d = 3.0000000000, h = 19.0000000000);
										}
									}
								}
							}
						}
						translate(v = [0, 0, -25.0000000000]) {
							translate(v = [-25.0000000000, -25.0000000000, -25.0000000000]) {
								cube(size = 50);
							}
						}
					}
				}
				mirror(v = [1, 0, 0]) {
					translate(v = [30.0000000000, 0, 0]) {
						intersection() {
							difference() {
								hull() {
									union() {
										rotate(a = [0, 90, 0]) {
											translate(v = [-0.0000000000, -0.0000000000, -12.0000000000]) {
												cylinder(d = 19.0000000000, h = 24.0000000000);
											}
										}
										translate(v = [0, 9.5000000000, 0]) {
											translate(v = [0.0000000000, 5.0000000000, 0.0000000000]) {
												translate(v = [-8.5000000000, -5.0000000000, -8.5000000000]) {
													cube(size = [17.0000000000, 10, 17.0000000000]);
												}
											}
										}
									}
								}
								translate(v = [0, 9.5000000000, 0]) {
									rotate(a = [-90, 0, 0]) {
										cylinder(d = 8.0000000000, h = 10);
									}
								}
								union() {
									translate(v = [6.5000000000, 12, 0]) {
										translate(v = [-0.0000000000, -0.0000000000, -9.5000000000]) {
											cylinder(d = 3.0000000000, h = 19.0000000000);
										}
									}
									mirror(v = [1, 0, 0]) {
										translate(v = [6.5000000000, 12, 0]) {
											translate(v = [-0.0000000000, -0.0000000000, -9.5000000000]) {
												cylinder(d = 3.0000000000, h = 19.0000000000);
											}
										}
									}
								}
							}
							translate(v = [0, 0, -25.0000000000]) {
								translate(v = [-25.0000000000, -25.0000000000, -25.0000000000]) {
									cube(size = 50);
								}
							}
						}
					}
				}
			}
		}
		translate(v = [0, 0, -6.5000000000]) {
			translate(v = [0, 0, -500.0000000000]) {
				translate(v = [-500.0000000000, -500.0000000000, -500.0000000000]) {
					cube(size = 1000);
				}
			}
		}
	}
	/* Holes Below*/
	difference(){
		union(){
			difference(){
				rotate(a = [0, 90, 0]) {
					translate(v = [-0.0000000000, -0.0000000000, -42.0000000000]) {
						cylinder(d = 15.0000000000, h = 84.0000000000);
					}
				}
			}
		}
	} /* End Holes */ 
}
/***********************************************
*********      SolidPython code:      **********
************************************************
 
from solid import *
from solid import extensions

class Object:
    pass


rod=Object()
rod.d=8.0

nema17 = Object()
nema17.side = 43
nema17.holeDistance = 31.0
nema17.screwD=3

bearing = Object()
bearing.outD = 15.0
bearing.length = 24.0

# to place the rod supports on top of the nema17
supportDepth=10
xSupport = ( cube([supportDepth,nema17.side,rod.d/2+1]).align('left','bottom')
             - cylinder(d=rod.d, h=supportDepth).rot(y=90).move(z=-.5)
             - cylinder(d=nema17.screwD,h=12).center().move(x=5,y=nema17.holeDistance/2.).mirrorCopy('y')
           ).rot(x=180)


bearingShape = cylinder(d=bearing.outD, h= bearing.length+1).center()

#minimal y cart, z motor will be placed in between the rails
yRodDistance = 60.0 # rounding up from nema17.side + bearing.outD
wall=2.0
cartWidth = 2*bearing.length
yBearings = bearingShape.move(z=bearing.length/2).mirrorCopy('z').move(x=yRodDistance/2).mirrorCopy('x')
yCart = ( ( cylinder(d= bearing.outD + 2*wall, h= cartWidth).center().rot(x=90).move(x=yRodDistance/2).mirrorCopy('x')
          + cube([yRodDistance, cartWidth, bearing.outD/2+wall]).align('bottom').move(z=-bearing.outD/2-wall)
          + cube([yRodDistance+bearing.outD+2*wall, cartWidth, bearing.outD/2+wall]).align('top').move(z=bearing.outD/2+wall)

          ) - yBearings.rot(x=90)
            - cube([nema17.side, cartWidth, bearing.outD/2-3]).align('top')
            - cube([nema17.side, cartWidth, bearing.outD]).align('bottom')
            - cube([yRodDistance+20, cartWidth, 3]).align('bottom').move(z=-bearing.outD/2-wall)
            - cylinder(d=3,h=10).move([nema17.holeDistance/2, nema17.holeDistance/2, -bearing.outD/2]).mirrorCopy('x').mirrorCopy('y').debug()
            - cylinder(d=nema17.holeDistance, h =10).move(z=-bearing.outD/2)
          )



#supports for holding the y rods in from the x bearings
bearingCover = cylinder(d= bearing.outD + 2*wall, h= bearing.length).center().rot(y=90)
baseBlock = hull( bearingCover
                  + cube([bearing.outD+2,10,bearing.outD+2]).align('back').move(y=bearing.outD/2+wall))
capScrews = cylinder(d=3., h=bearing.outD + 2*wall).center().move(x=rod.d/2+3./2+1,y=12).mirrorCopy('x')
rodHole = cylinder(d=rod.d,h=10).rot(x=-90).move(y=bearing.outD/2+wall)
#capTopBase = cube([bearing.length, 11, bearing.outD]).center('x').move(y=bearing.outD/2+wall)
#capTopBase = polygon([[0,0],[0,10],[10,10],[10,-3]]).linear_extrude(bearing.length,True).rot(y=-90).move(y=bearing.outD/2+wall)
capTopBase = cube([bearing.length, 20, bearing.outD]).center('x')-bearingCover

baseSupport = baseBlock - rodHole - capScrews

ySupport = part(cylinder(d= bearing.outD + 2*wall, h= yRodDistance + bearing.length).center().rot(y=90)
                 - hole(cylinder(d= bearing.outD, h= yRodDistance + bearing.length).center().rot(y=90))
                 + ( baseSupport
                     * cube(50).align('top')
                   ).move(x=yRodDistance/2).mirrorCopy('x')
                 - cube(1000).align('top').move(z=-bearing.outD/2+1)
                 )

ySupportRodCap = ( baseSupport
                * capTopBase
                - cube([bearing.length,50,1]).align('bottom')
                - cube([bearing.length,10,10]).center('xy')
                )

#support for z rods placed on the falt surface of the yCart above the bearings
zRodDistance = 31.
rodSupport = (cylinder(d=rod.d + 3, h=6) - cylinder(d=rod.d, h=6+1)).move(y=zRodDistance / 2)
beltGuide = ( cube([bearing.outD, bearing.length-zRodDistance/2, 6]).center('x')
              - cylinder(d=rod.d + 6,h=6+1)
              - cube([1,20,6+1]).center('x')).move(y=zRodDistance/2)
zSupport = ( cube([bearing.outD,2*bearing.length,wall]).center('xy')
             + rodSupport.move(z=wall).mirrorCopy('y')
             + beltGuide.move(z=wall).mirrorCopy('y')
           )

zstepper = cube([nema17.side,nema17.side,34]).center('xy').move(z=-bearing.outD/2).color('gray')


yCartZSupport = yCart + zSupport.move(x=yRodDistance/2 ,z=bearing.outD/2)


scene = ( xSupport.move(x=-100)
          + ySupport + ySupportRodCap.move(x=-yRodDistance / 2, z=3)
          + (yCartZSupport ).move(y=50)
        )
scene = yCartZSupport

scad_render_to_file(scene, 'routerParts.scad', '$fs=.75;')

scad_render_to_file(xSupport, 'routerParts_xSupport.scad', '$fs=.75;')
scad_render_to_file(ySupport, 'routerParts_ySupport.scad', '$fs=.75;')
scad_render_to_file(ySupportRodCap, 'routerParts_ySupportCap.scad', '$fs=.75;')
scad_render_to_file(yCartZSupport, 'routerParts_yCart.scad', '$fs=.75;')
 
 
************************************************/
