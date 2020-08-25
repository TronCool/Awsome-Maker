$fn=100;
difference(){
  cube([89,59,18.5]);
  translate([1,1,1]) cube([89,57,16.5]);
  translate([89,29.5,-1]) cylinder(20, r=7);
  translate([-1,-1.5,16.5]) rotate([45,0,0]) cube([91,10,2]);
  translate([-1,-2.5,0]) rotate([-45,0,0]) cube([91,10,2]);
  translate([-1,55.5,21.5]) rotate([-45,0,0]) cube([91,10,2]);
  translate([-1,55.5,-6]) rotate([45,0,0]) cube([91,10,2]);
  translate([-4.5,-1,2]) rotate([0,45,0]) cube([10,60,2]);
  translate([-4.5,-1,13.5]) rotate([0,-45,0]) cube([10,60,2]);
  translate([2.75,-5,-1]) rotate([0,0,45]) cube([2,10,20]);
  translate([-6.25,55,-1]) rotate([0,0,-45]) cube([2,10,20]);
  hull(){
    translate([-1,12.5,8.75]) rotate([0,90,0]) cylinder(5, r=5);
    translate([-1,45,8.75]) rotate([0,90,0]) cylinder(5, r=5);
  }
}

