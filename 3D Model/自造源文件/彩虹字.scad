$fn=100; //精度
rotate([0,-90,0]) rotate_extrude(angle=90){ //下面一行第一个20调整弧度大小
  translate([20,20,0]) rotate([0,0,-90]) text("可为的基地", size=30, font="FZZJ\\-LongYTJW:style=Regular");
}
