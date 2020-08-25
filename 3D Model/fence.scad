$fn=50;
module fence(x,y) {
  translate([x,y,0]) rotate([90,0,0]) linear_extrude(height=3){
    translate([0,50,0]) circle(10);
    translate([-10,0,0]) square([20,50]);
  }
}

for(a=[0:10]) {
  fence(a*30,0);
  fence(a*30,80);
}

difference(){
  translate([-15, 0, 0]) cube([330,77,40]);
  translate([-10, 5, 10]) cube([320,67,31]);
  translate([50, 40, -1]) cylinder(50,r=6);
  translate([150, 40, -1]) cylinder(50,r=6);
  translate([250, 40, -1]) cylinder(50,r=6);
}