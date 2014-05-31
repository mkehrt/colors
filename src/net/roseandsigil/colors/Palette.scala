package net.roseandsigil.colors

import java.awt


class PaletteWindowListener extends awt.event.WindowAdapter {
  override def windowClosing(evt: awt.event.WindowEvent) {
    System.exit(0)
  }
}

class Swatch(index: Int, outOf: Int) extends awt.Label {
  val w = Palette.Width / outOf
  val h = Palette.Height
  val xx = w * index
  val yy = 0


  setBounds(xx, yy, w, h)

  def clamp(c: Int) = {
    if (c < 0) 0
    else if (c > 255) 255
    else c
  }

  def setColor(ll: Double, r: Double) {
    import java.lang.Math
    val radians = index.toFloat / outOf.toFloat * 2 * Math.PI
    val l = ll * 100
    val a = 100 * Math.sin(radians) * r
    val b = 100 * Math.cos(radians) * r
    val rgb = Palette.c.LABtoRGB(l, a, b)
    println("%f,%f,%f -> %d,%d,%d".format(l, a, b, rgb(0), rgb(1), rgb(2)))
    val color = new awt.Color(clamp(rgb(0)), clamp(rgb(1)), clamp(rgb(2)))
    setBackground(color)
  }

  setColor(1.0, 0.3)
  setVisible(true)
}

class PaletteFrame extends awt.Frame {
  setSize(Palette.Width, Palette.Height)

  addWindowListener(new PaletteWindowListener)

  for(i <- 0 until Palette.Number) {
    val s = new Swatch(i, Palette.Number)
    add(s)
  }
  
  setBackground( new awt.Color(100, 100, 100))
  setVisible(true)
}

object Palette {
  val Width = 400
  val Height = 300

  val Number = 6

  val c = new ColorSpaceConverter

  def main(args: Array[String]) {
    val p = new PaletteFrame
  }
}
