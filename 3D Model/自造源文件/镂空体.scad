difference(){
  sphere(r = 2, $fn=10);
  sphere(r = 1.88, $fn=20);
  translate([-2,-2,1.2]) cube([5,5,2]);
  translate([-2,-2,-3.2]) cube([5,5,2]);
}