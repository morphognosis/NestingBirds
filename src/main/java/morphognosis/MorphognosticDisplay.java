// For conditions of distribution and use, see copyright notice in Morphognosis.java

//  Morphognostic display.

package morphognosis;

import java.awt.BorderLayout;
import java.awt.Choice;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Random;
import java.util.Vector;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class MorphognosticDisplay extends JPanel implements ItemListener, ActionListener
{
   private static final long serialVersionUID = 0L;

   // Morphognostic.
   Morphognostic morphognostic;

   // Components.
   Choice                neighborhoodChoice;
   Choice                sectorChoice;
   Vector<SectorDisplay> displays;
   JButton               showButton;
   JButton               hideButton;

   // Constructor.
   public MorphognosticDisplay(Morphognostic morphognostic)
   {
      this.morphognostic = morphognostic;
      setLayout(new BorderLayout());
      setBorder(BorderFactory.createTitledBorder(
                   BorderFactory.createLineBorder(Color.black), "Morphognostic"));
      JPanel morphognosticPanel = new JPanel();
      morphognosticPanel.setLayout(new BorderLayout());
      add(morphognosticPanel);
      JPanel neighborhoodPanel = new JPanel();
      neighborhoodPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
      morphognosticPanel.add(neighborhoodPanel, BorderLayout.NORTH);
      neighborhoodPanel.add(new JLabel("Neighborhood:"));
      neighborhoodChoice = new Choice();
      for (int i = 0, j = morphognostic.neighborhoods.size(); i < j; i++)
      {
         neighborhoodChoice.add(i + " ");
      }
      neighborhoodChoice.addItemListener(this);
      neighborhoodPanel.add(neighborhoodChoice);
      neighborhoodPanel.add(new JLabel("Sector:"));
      sectorChoice = new Choice();
      int n = neighborhoodChoice.getSelectedIndex();
      if (n != -1)
      {
         sectorChoice.add("all");
         for (int x = 0, d = morphognostic.neighborhoods.get(n).sectors.length; x < d; x++)
         {
            for (int y = 0; y < d; y++)
            {
               sectorChoice.add("[" + x + "," + y + "] ");
            }
         }
      }
      neighborhoodPanel.add(sectorChoice);
      JPanel showPanel = new JPanel();
      showPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
      morphognosticPanel.add(showPanel, BorderLayout.SOUTH);
      showButton = new JButton("Show");
      showButton.addActionListener(this);
      showPanel.add(showButton);
      hideButton = new JButton("Hide");
      hideButton.addActionListener(this);
      showPanel.add(hideButton);
      displays = new Vector<SectorDisplay>();
   }


   // Choice listener.
   public void itemStateChanged(ItemEvent evt)
   {
      if ((Choice)evt.getSource() == neighborhoodChoice)
      {
         int n = neighborhoodChoice.getSelectedIndex();
         sectorChoice.removeAll();
         if (n != -1)
         {
            sectorChoice.add("all");
            for (int x = 0, d = morphognostic.neighborhoods.get(n).sectors.length; x < d; x++)
            {
               for (int y = 0; y < d; y++)
               {
                  sectorChoice.add("[" + x + "," + y + "] ");
               }
            }
         }
      }
   }


   // Button listener.
   public void actionPerformed(ActionEvent evt)
   {
      if ((JButton)evt.getSource() == showButton)
      {
         showSectors();
      }
      else if ((JButton)evt.getSource() == hideButton)
      {
         hideSectors();
      }
   }


   // Show sectors.
   void showSectors()
   {
      int n = neighborhoodChoice.getSelectedIndex();
      int s = sectorChoice.getSelectedIndex();

      if ((n != -1) && (s != -1))
      {
         int x = -1;
         int y = -1;
         if (s != 0)
         {
            String i = sectorChoice.getItem(s).trim();
            i = i.substring(1, i.length() - 1);
            String d = ",";
            String[] j = i.split(d);
            x          = Integer.parseInt(j[0]);
            y          = Integer.parseInt(j[1]);
         }
         Dimension dim  = Toolkit.getDefaultToolkit().getScreenSize();
         int       offx = new Random().nextInt((int)((float)dim.width * 0.1f));
         int       offy = new Random().nextInt((int)((float)dim.height * 0.1f));
         for (int x2 = 0, d = morphognostic.neighborhoods.get(n).sectors.length; x2 < d; x2++)
         {
            for (int y2 = 0; y2 < d; y2++)
            {
               if ((s == 0) || ((x2 == x) && (y2 == y)))
               {
                  boolean found = false;
                  for (SectorDisplay display : displays)
                  {
                     if ((display.neighborhoodIndex == n) &&
                         (display.sectorXindex == x2) &&
                         (display.sectorYindex == y2))
                     {
                        found = true;
                        break;
                     }
                  }
                  if (!found)
                  {
                     SectorDisplay display = new SectorDisplay(this, n, x2, y2);
                     displays.add(display);
                     int w = display.getSize().width;
                     int h = display.getSize().height;
                     display.setLocation(offx + (x2 * w), offy + (y2 * h));
                     display.open();
                  }
               }
            }
         }
      }
   }


   // Hide sectors.
   void hideSectors()
   {
      int n = neighborhoodChoice.getSelectedIndex();
      int s = sectorChoice.getSelectedIndex();

      if ((n != -1) && (s != -1))
      {
         int x = -1;
         int y = -1;
         if (s != 0)
         {
            String i = sectorChoice.getItem(s).trim();
            i = i.substring(1, i.length() - 1);
            String d = ",";
            String[] j = i.split(d);
            x          = Integer.parseInt(j[0]);
            y          = Integer.parseInt(j[1]);
         }
         for (int x2 = 0, d = morphognostic.neighborhoods.get(n).sectors.length; x2 < d; x2++)
         {
            for (int y2 = 0; y2 < d; y2++)
            {
               if ((s == 0) || ((x2 == x) && (y2 == y)))
               {
                  boolean done = false;
                  while (!done)
                  {
                     done = true;
                     for (SectorDisplay display : displays)
                     {
                        if ((display.neighborhoodIndex == n) &&
                            (display.sectorXindex == x2) &&
                            (display.sectorYindex == y2))
                        {
                           display.close();
                           displays.remove(display);
                           done = false;
                           break;
                        }
                     }
                  }
               }
            }
         }
      }
   }


   // Close.
   public void close()
   {
      while (displays.size() > 0)
      {
         displays.get(0).close();
      }
   }


   // Close display callback.
   void closeDisplay(int neighborhoodIndex, int sectorXindex, int sectorYindex)
   {
      for (SectorDisplay display : displays)
      {
         if ((display.neighborhoodIndex == neighborhoodIndex) &&
             (display.sectorXindex == sectorXindex) && (display.sectorYindex == sectorYindex))
         {
            displays.remove(display);
            break;
         }
      }
   }
}
