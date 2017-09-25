from solid import *
from solid import extensions

class Object:
    pass


rod=Object()
rod.d=8.0

nema17 = Object()
nema17.side = 43.
nema17.holeDistance = 31.0
nema17.screwD= 3.
nema17.height = 34. #47 for the long ones, 34 for the short ones


bearing = Object()
bearing.outD = 15.0
bearing.length = 24.0

# to place the rod supports on top of the nema17
supportDepth=10
xRodHole = cylinder(d=rod.d, h=20).rot(y=90)
xSupport = ( cube([supportDepth,nema17.side,rod.d/2+1]).align('left','bottom')
             - xRodHole.move(z=-.5)
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


def xTailBase(h):
    return hull([cylinder(d=3+4,h=h).move(x= 4, y=3./2+rod.d/2+1).mirrorCopy('y'),
                 cylinder(d=4+6, h=h).move(x=-3)
                ]).move(z=.5)

capScrewHoles = cylinder(d=3,h=100).center().move(x= 4, y=3./2+rod.d/2+1).mirrorCopy('y')
pulleyScrewHole = cylinder(d=4,h=100).center().move(x= -3)
nutCuts = (cylinder(d=3+4,h=nema17.height-7).move(x= 4, y=3./2+rod.d/2+1).mirrorCopy('y')
          + cylinder(d=4+6, h=nema17.height-7).move(x=-3)).move(z=7)
xTailSupport = ( xTailBase(nema17.height+1.5+rod.d/2.)
                 - xRodHole
                 - capScrewHoles
                 - pulleyScrewHole
                 - nutCuts
               )
xTailCap = ( xTailBase(7)
             - xRodHole
             - capScrewHoles
             - pulleyScrewHole
            )

def xFrontBase(h):
    return hull([cylinder(d=3+4,h=h).move(y=3./2+rod.d/2+1).mirrorCopy('y'),
                ]).move(x=3.5,z=.5)

capScrewHoles = cylinder(d=3,h=100).center().move(x=3.5,y=3./2+rod.d/2+1).mirrorCopy('y')
nutCuts = (cylinder(d=3+4,h=nema17.height-7).move(x=3.5,y=3./2+rod.d/2+1).mirrorCopy('y')
          ).move(z=7)
xSupportH=nema17.height+1.5+rod.d/2.
nemaSupport = ( hull(cylinder(d=3+4,h=1.5).move(x=-(nema17.side-nema17.holeDistance)/2,y=nema17.holeDistance/2,z=xSupportH-1.5-nema17.height).mirrorCopy('y')
                     + cylinder(d=3+4,h=xSupportH-nema17.height-.5).move(y=3./2+rod.d/2+1).mirrorCopy('y').move(x=3.5,z=.5)
                    )
                - cylinder(d=3,h=10).move(x=-(nema17.side-nema17.holeDistance)/2,y=nema17.holeDistance/2).mirrorCopy('y'))
xFrontSupport = ( xFrontBase(xSupportH)
                 + nemaSupport
                 - xRodHole.move(x=-10)
                 - capScrewHoles
                 - nutCuts
               )
xFrontCap = ( xFrontBase(7)
             - xRodHole.move(x=-10)
             - capScrewHoles
            )


zTopH=5.
zBearingD = 16.
zTopBase = hull(cylinder(d=rod.d+6,h=zTopH).move(y=zRodDistance/2).mirrorCopy('y')
            +cylinder(d=zBearingD+6,h=zTopH).move(x=-yRodDistance/2))
zHoles =  (cylinder(d=rod.d,h=zTopH).move(y=zRodDistance/2).mirrorCopy('y')
           + cylinder(d=zBearingD,h=zTopH).move(x=-yRodDistance/2))
zTop = zTopBase - zHoles




zNutD0 = 10.
zNutD1 = 22.
zNutH0 = 10.
drillD = 36.

base = ( hull(cylinder(d=bearing.outD+6,h=bearing.length).move(y=zRodDistance/2).mirrorCopy('y')
              + cylinder(d=zNutD1+2,h=bearing.length).move(x=-yRodDistance/2)
              + cylinder(d=drillD+6,h=bearing.length).move(x=drillD/2+5)
              + cube([.1,25,zNutH0]).center('y').move(x=drillD+5)
              )
            + cube([8,25,zNutH0]).center('y').move(x=drillD+5)
        )

holes =  (cylinder(d=bearing.outD, h=bearing.length).move(y=zRodDistance/2).mirrorCopy('y')
           + cylinder(d=zNutD0+1,h=bearing.length).move(x=-yRodDistance/2)
           + cylinder(d=drillD,h=bearing.length).move(x=drillD/2+5)
           + cube([20,2,zNutH0]).center('xy').move(x=drillD+7)
           + cylinder(d=3,h=30).center().rot(x=90).move(x=drillD+9,z=zNutH0/2).debug()
           )

zNutDrills = cylinder(d=3,h=bearing.length).move(x=17./2).rot(z=45).mirrorCopy('y').mirrorCopy('x').move(x=-yRodDistance/2)

def ushape(p,f=.5):
    return (cylinder(d=100.*f,h=100.).center().rot(x=90)-cube([500.*f,100,100]).center('yz')
            +cube([50.*(1-f),100.,100.*f]).center('yz')

           ).move(x=(.5-f)*-50).resize(p)

pD=[40,60,30]
pN=[35,60,30]
profileCuts = (ushape(pD,.5).move(x=pD[0]/2+bearing.outD/2,z=pD[2]/2+zNutH0)+
           ushape(pN,.4).mirror('x').move(x=-pN[0]/2-bearing.outD/2,z=pN[2]/2+zNutH0)
          )

sideCuts = cube([100,20,bearing.length]).center('x').move(y=zRodDistance/2+bearing.outD/2-1).mirrorCopy('y')

zDrillCart_ = base - holes - zNutDrills  - profileCuts

zDrillCart = zDrillCart_.copy() #- sideCuts
zDrillCart0 = zDrillCart_.copy().intersection(cube([60,60,bearing.length]).center('y').move(x=-bearing.outD/2+1))

scene = xFrontSupport +xFrontCap.move(x=-20)


'''
scene = ( xSupport.move(x=-100)
          + ySupport + ySupportRodCap.move(x=-yRodDistance / 2, z=3)
          + (yCartZSupport ).move(y=50)
        )
'''
scad_render_to_file(scene, 'routerParts.scad', '$fs=.75;')

#scad_render_to_file(xSupport, 'routerParts_xSupport.scad', '$fs=.75;')
#scad_render_to_file(ySupport, 'routerParts_ySupport.scad', '$fs=.75;')
#scad_render_to_file(ySupportRodCap, 'routerParts_ySupportCap.scad', '$fs=.75;')
#scad_render_to_file(yCartZSupport, 'routerParts_yCart.scad', '$fs=.75;')
#scad_render_to_file( xTailSupport.rot(y=90), 'routerParts_xTailSupport.scad', '$fs=.75;')
#scad_render_to_file( xTailCap.rot(y=90), 'routerParts_xTailCap.scad', '$fs=.75;')
scad_render_to_file( xFrontSupport.rot(y=90), 'routerParts_xFrontSupport.scad', '$fs=.75;')
scad_render_to_file( xFrontCap.rot(y=90), 'routerParts_xFrontCap.scad', '$fs=.75;')
#scad_render_to_file( zTop, 'routerParts_zTop.scad', '$fs=.75;')
#scad_render_to_file( zDrillCart, 'routerParts_zDrillCart.scad', '$fs=.75;')
#scad_render_to_file( zDrillCart0, 'routerParts_zDrillCart0.scad', '$fs=.75;')
