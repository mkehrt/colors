package net.roseandsigil.colors

import java.awt


class PaletteWindowListener extends awt.event.WindowAdapter {
  override def windowClosing(evt: awt.event.WindowEvent) {
    System.exit(0)
  }
}

class Swatch(index: Int, outOf: Int) extends awt.Label {
  val w = Palette.Width / outOf
  val h = Palette.Height / 3
  val xx = w * index
  val yy = 0

  println("%d,%d %d,%d".format(xx, yy, w, h))

  setBounds(xx, yy, w, h)

  def setColor(l: Double, r: Double) {
    val c = index.toFloat / outOf.toFloat
    val color = new awt.Color(c, c, c)
    setBackground(color)
  }

  setColor(0.0, 0.0)
  setVisible(true)
}

class PaletteFrame extends awt.Frame {
  setSize(Palette.Width, Palette.Height)

  addWindowListener(new PaletteWindowListener)

  for(i <- 0 until Palette.Number) {
    val s = new Swatch(i, Palette.Number)
    add(s)
  }

  setVisible(true)
}

object Palette {
  val Width = 400
  val Height = 400

  val Number = 6

  val c = new ColorSpaceConverter

  def main(args: Array[String]) {
    val p = new PaletteFrame
  }
}
