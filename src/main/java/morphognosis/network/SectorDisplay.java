// For conditions of distribution and use, see copyright notice in Morphognosis.java

package morphognosis.network;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Random;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

public class SectorDisplay extends JFrame implements Runnable
{
   private static final long serialVersionUID = 0L;

   // Display.
   MorphognosticDisplay display;

   // Neighborhood and sector.
   int                               neighborhoodIndex;
   int                               sectorXindex, sectorYindex;
   Morphognostic                     morphognostic;
   Morphognostic.Neighborhood        neighborhood;
   Morphognostic.Neighborhood.Sector sector;

   // Display.
   static final int       DISPLAY_UPDATE_DELAY_MS = 50;
   static final Dimension displaySize             = new Dimension(275, 200);
   static final Dimension canvasSize = new Dimension(275, 175);
   Canvas                 canvas;
   Graphics               canvasGraphics;
   Image     image;
   Graphics  imageGraphics;
   Dimension imageSize;
   Font      imageFont;
   Thread    displayThread;
   enum DISPLAY_MODE { VALUES, NAMES };
   DISPLAY_MODE displayMode;

   // Constructor.
   public SectorDisplay(MorphognosticDisplay display,
                        int neighborhoodIndex, int sectorXindex, int sectorYindex)
   {
      this.display           = display;
      this.neighborhoodIndex = neighborhoodIndex;
      this.sectorXindex      = sectorXindex;
      this.sectorYindex      = sectorYindex;
      morphognostic          = display.morphognostic;
      neighborhood           = morphognostic.neighborhoods.get(neighborhoodIndex);
      sector = neighborhood.sectors[sectorXindex][sectorYindex];

      setTitle("N=" + neighborhoodIndex + " D=" + neighborhood.duration +
               " S=[" + sectorXindex + "," + sectorYindex + "]");
      addWindowListener(new WindowAdapter()
                        {
                           public void windowClosing(WindowEvent e) { close(); }
                        }
                        );
      JPanel basePanel = (JPanel)getContentPane();
      basePanel.setLayout(new BorderLayout());
      canvas = new Canvas();
      canvas.setBounds(0, 0, canvasSize.width, canvasSize.height);
      basePanel.add(canvas, BorderLayout.NORTH);
      JPanel modePanel = new JPanel();
      modePanel.setLayout(new FlowLayout());
      basePanel.add(modePanel, BorderLayout.SOUTH);
      JRadioButton values = new JRadioButton("Values", true);
      displayMode = DISPLAY_MODE.VALUES;
      values.addActionListener(new ActionListener()
                               {
                                  @Override
                                  public void actionPerformed(ActionEvent e)
                                  {
                                     displayMode = DISPLAY_MODE.VALUES;
                                  }
                               }
                               );
      JRadioButton names = new JRadioButton("Names");
      names.addActionListener(new ActionListener()
                              {
                                 @Override
                                 public void actionPerformed(ActionEvent e)
                                 {
                                    displayMode = DISPLAY_MODE.NAMES;
                                 }
                              }
                              );
      ButtonGroup modeGroup = new ButtonGroup();
      modeGroup.add(values);
      modeGroup.add(names);
      modePanel.add(values);
      modePanel.add(names);
      pack();
      setVisible(false);

      // Get canvas image.
      canvasGraphics = canvas.getGraphics();
      image          = createImage(canvasSize.width, canvasSize.height);
      imageGraphics  = image.getGraphics();
      imageSize      = canvasSize;
      imageFont      = new Font("Ariel", Font.PLAIN, 10);
      imageGraphics.setFont(imageFont);

      // Create display thread.
      displayThread = new Thread(this);
      displayThread.setPriority(Thread.MIN_PRIORITY);
      displayThread.start();
   }


   // Open display.
   void open()
   {
      setVisible(true);
   }


   // Close display.
   void close()
   {
      setVisible(false);
      display.closeDisplay(neighborhoodIndex, sectorXindex, sectorYindex);
   }


   // Run.
   public void run()
   {
      // Display update loop.
      while (Thread.currentThread() == displayThread &&
             !displayThread.isInterrupted())
      {
         updateDisplay();

         try
         {
            Thread.sleep(DISPLAY_UPDATE_DELAY_MS);
         }
         catch (InterruptedException e) {
            break;
         }
      }
   }


   // Update display.
   public void updateDisplay()
   {
      int   d, i, j, n, h;
      float fx, fw;

      // Get colors
      Color[] colors = new Color[morphognostic.eventDimensions];
      for (d = 0; d < morphognostic.eventDimensions; d++)
      {
         colors[d] = getEventDimensionColor(d);
      }

      if (displayMode == DISPLAY_MODE.VALUES)
      {
         // Draw dimension values.
         imageGraphics.setColor(Color.gray);
         imageGraphics.fillRect(0, 0, imageSize.width, imageSize.height);

         // Draw values.
         fw = (float)imageSize.width / (float)morphognostic.eventDimensions;
         fx = 0.0f;
         for (i = d = 0; d < morphognostic.eventDimensions; d++)
         {
            if ((neighborhood.eventDimensionMap == null) || neighborhood.eventDimensionMap[d])
            {
               imageGraphics.setColor(colors[d]);
               h = (int)((float)imageSize.height * sector.getEventDimensionValue(d));
               imageGraphics.fillRect((int)fx, imageSize.height - h, (int)(fw + 1.0), h);
            }
         }
         imageGraphics.setColor(Color.black);
         imageGraphics.drawLine(0, 0, imageSize.width, 0);
         imageGraphics.drawLine(0, imageSize.height - 1, imageSize.width, imageSize.height - 1);
         for (i = 0, j = morphognostic.eventDimensions - 1, fx = fw; i < j; i++, fx += fw)
         {
            imageGraphics.drawLine((int)fx, 0, (int)fx, imageSize.height);
         }
      }
      else
      {
         // Draw dimension names.
         imageGraphics.setColor(Color.black);
         imageGraphics.fillRect(0, 0, imageSize.width, imageSize.height);

         imageGraphics.setColor(Color.white);
         if (morphognostic.eventDimensionNames == null)
         {
            imageGraphics.drawString("event names unavailable", 0, 5);
         }
         else
         {
            int y = 9;
            for (i = 0; i < morphognostic.eventDimensionNames.length; i++)
            {
               if ((neighborhood.eventDimensionMap == null) || neighborhood.eventDimensionMap[i])
               {
                  String name = morphognostic.eventDimensionNames[i];
                  if (name != null)
                  {
                     imageGraphics.drawString(name, 0, y);
                  }
                  else
                  {
                     imageGraphics.drawString("unnamed", 0, y);
                  }
                  int x = canvasGraphics.getFontMetrics().stringWidth(name);
                  imageGraphics.setColor(colors[i]);
                  imageGraphics.fillRect(x, y - 5, 5, 5);
                  imageGraphics.setColor(Color.white);
                  y += 9;
               }
            }
         }
      }
      canvasGraphics.drawImage(image, 0, 0, this);
   }


   // Get event dimension color.
   public static Color getEventDimensionColor(int dimension)
   {
      Random random = new Random();

      random.setSeed(((dimension + 3) * 1000));
      float r = random.nextFloat();
      float g = random.nextFloat();
      float b = random.nextFloat();
      return(new Color(r, g, b));
   }
}
