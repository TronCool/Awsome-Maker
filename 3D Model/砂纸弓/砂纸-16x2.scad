$fn=200;

color("#f00") side(-45);
color("#f0f") rotate([0,180,0]) side(-45);
translate([-45,15,-10]) cube([90,5,20]);

module side(x){ //弓反曲
    translate([x,0,0]) union(){
        intersection(){
            difference(){
                cylinder(r=20,h=20,center=true);
                cylinder(r=15,h=21,center=true);
            }
            translate([-20,0,-10.5]) cube([20,20,21]);
        }
        translate([-20,-17.5,-10]) cube([5,17.5,20]);
        translate([-17.5, -17.5, -10]) cylinder(r=2.5,h=20);
        translate([-20, -20, -10]) cube([2.5,5,20]);
        translate([-29.2, -8.8, 0]) rotate(45) board();
    }    
}

module board() { //搓衣板
    union(){
        translate([-2,-13,-10]) cube([4,16,20]);
        translate([-2,-10, 0]) cylinder(r=1,h=20,center=true);
        translate([-2,-5, 0]) cylinder(r=1,h=20,center=true);
        translate([-2,0, 0]) cylinder(r=1,h=20,center=true);
        translate([2,-10, 0]) cylinder(r=1,h=20,center=true);
        translate([2,-5, 0]) cylinder(r=1,h=20,center=true);
        translate([2,0, 0]) cylinder(r=1,h=20,center=true);
        translate([0, 3, 0]) cylinder(r=2,h=20,center=true);
        translate([0, -13, 0]) cylinder(r=2,h=20,center=true);
    }
}
