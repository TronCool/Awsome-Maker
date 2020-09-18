$fn=100;
difference(){
  cube([89,61,19], center=true);
  translate([1,0,0]) cube([89,59,17], center=true);
  translate([44.5,0]) cylinder(25, r=7, center=true);
  hull(){
    translate([-44,14,0]) rotate([0,90,0]) cylinder(5, r=5, center=true);
    translate([-44,-14,0]) rotate([0,90,0]) cylinder(5, r=5, center=true);
  }
  hull(){
    translate([-30,5,-1]) cylinder(25, r=3, center=true);
    translate([-30,-5,-1]) cylinder(25, r=3, center=true);
    translate([6,0,-1]) cylinder(25, r=10, center=true);
  }
  hull(){
    translate([22,0,-1]) cylinder(25, r=2.5, center=true);
    translate([2,15,-1]) cylinder(25, r=2.5, center=true);
    translate([2,-15,-1]) cylinder(25, r=2.5, center=true);
  }
  translate([0,30.75,9.75]) rotate([45,0,0]) cube([95,1,1], center=true);
  translate([0,-30.75,9.75]) rotate([45,0,0]) cube([95,1,1], center=true);
  translate([0,30.75,-9.75]) rotate([45,0,0]) cube([95,1,1], center=true);
  translate([0,-30.75,-9.75]) rotate([45,0,0]) cube([95,1,1], center=true);
  translate([-44.75,0,9.75]) rotate([0,45,0]) cube([1,65,1], center=true);
  translate([-44.75,0,-9.75]) rotate([0,45,0]) cube([1,65,1], center=true);
  translate([-44.75,30.75,0]) rotate([0,0,45]) cube([1,1,25], center=true);
  translate([-44.75,-30.75,0]) rotate([0,0,45]) cube([1,1,25], center=true);
}

