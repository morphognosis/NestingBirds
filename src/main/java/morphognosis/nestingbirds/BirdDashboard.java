// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting bird dashboard.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import morphognosis.nestingbirds.Bird.ORIENTATION;

public class BirdDashboard extends JFrame
{
   private static final long serialVersionUID = 0L;

   // Components.
   StatusPanel status;
   DriverPanel driver;

   // Target bird.
   Bird bird;

   // Environment display.
   EnvironmentDisplay environmentDisplay;

   // Constructor.
   public BirdDashboard(Bird bird, EnvironmentDisplay environmentDisplay)
   {
      this.bird = bird;
      this.environmentDisplay = environmentDisplay;

      if (bird.gender == Bird.MALE)
      {
         setTitle("Male bird");
      }
      else
      {
         setTitle("Female bird");
      }
      addWindowListener(new WindowAdapter()
                        {
                           public void windowClosing(WindowEvent e) { close(); }
                        }
                        );
      JPanel basePanel = (JPanel)getContentPane();
      basePanel.setLayout(new BoxLayout(basePanel, BoxLayout.Y_AXIS));
      status = new StatusPanel();
      basePanel.add(status);
      driver = new DriverPanel();
      basePanel.add(driver);
      pack();
      setLocation();
      setVisible(false);
      update();
   }


   void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (int)((float)(dim.width - w) * 0.9f);
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }


   // Update dashboard.
   void update()
   {
      // Update status.
      status.update();
   }


   // Open the dashboard.
   void open()
   {
      setVisible(true);
   }


   // Close the dashboard.
   void close()
   {
      setVisible(false);
   }


   // Status panel.
   class StatusPanel extends JPanel
   {
      private static final long serialVersionUID = 0L;

      // Components.
      JTextField localeText;
      JTextField objectText;
      JTextField wantFoodText;
      JTextField wantStoneText;
      JTextField orientationText;
      JTextField foodText;
      JTextField hasObjectText;
      JTextField responseText;

      // Constructor.
      public StatusPanel()
      {
         setLayout(new BorderLayout());
         setBorder(BorderFactory.createTitledBorder(
                      BorderFactory.createLineBorder(Color.black),
                      "State"));
         JPanel sensorsPanel = new JPanel();
         sensorsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Sensors"));
         add(sensorsPanel, BorderLayout.NORTH);
         sensorsPanel.add(new JLabel("Locale:"));
         localeText = new JTextField(10);
         localeText.setEditable(false);
         sensorsPanel.add(localeText);
         sensorsPanel.add(new JLabel("Object:"));
         objectText = new JTextField(10);
         objectText.setEditable(false);
         sensorsPanel.add(objectText);
         if (bird.gender == Bird.MALE)
         {
            sensorsPanel.add(new JLabel("Want food:"));
            wantFoodText = new JTextField(5);
            wantFoodText.setEditable(false);
            sensorsPanel.add(wantFoodText);
            sensorsPanel.add(new JLabel("Want stone:"));
            wantStoneText = new JTextField(5);
            wantStoneText.setEditable(false);
            sensorsPanel.add(wantStoneText);
         }
         JPanel responsePanel = new JPanel();
         responsePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         responsePanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Response"));
         add(responsePanel, BorderLayout.CENTER);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(25);
         responseText.setEditable(false);
         responsePanel.add(responseText);
         JPanel statePanel = new JPanel();
         statePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         statePanel.setBorder(BorderFactory.createTitledBorder(
                                 BorderFactory.createLineBorder(Color.black),
                                 "Internal"));
         add(statePanel, BorderLayout.SOUTH);
         statePanel.add(new JLabel("Orientation: "));
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         statePanel.add(orientationText);
         statePanel.add(new JLabel("Food: "));
         foodText = new JTextField(10);
         foodText.setEditable(false);
         statePanel.add(foodText);
         statePanel.add(new JLabel("Has object: "));
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         statePanel.add(hasObjectText);
      }


      // Update.
      public void update()
      {
         // Get sensor values.
         switch (bird.sensors[Bird.LOCALE_SENSOR])
         {
         case Environment.LOCALE.DESERT:
            localeText.setText("desert");
            break;

         case Environment.LOCALE.FOREST:
            localeText.setText("forest");
            break;

         case Environment.LOCALE.PLAIN:
            localeText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.OBJECT_SENSOR])
         {
         case Environment.OBJECT.NO_OBJECT:
            objectText.setText("none");
            break;

         case Environment.OBJECT.MOUSE:
            objectText.setText("mouse");
            break;

         case Environment.OBJECT.STONE:
            objectText.setText("stone");
            break;

         case Environment.OBJECT.EGG:
            objectText.setText("egg");
            break;
         }
         if (bird.gender == Bird.MALE)
         {
            if (bird.sensors[MaleBird.WANT_FOOD_SENSOR] == 0)
            {
               wantFoodText.setText("false");
            }
            else
            {
               wantFoodText.setText("true");
            }
            if (bird.sensors[MaleBird.WANT_STONE_SENSOR] == 0)
            {
               wantStoneText.setText("false");
            }
            else
            {
               wantStoneText.setText("true");
            }
         }

         // Internal state.
         switch (bird.orientation)
         {
         case ORIENTATION.NORTH:
            orientationText.setText("NORTH");
            break;

         case ORIENTATION.SOUTH:
            orientationText.setText("SOUTH");
            break;

         case ORIENTATION.EAST:
            orientationText.setText("EAST");
            break;

         case ORIENTATION.WEST:
            orientationText.setText("WEST");
            break;
         }
         foodText.setText(bird.food + "");
         switch (bird.hasObject)
         {
         case Environment.OBJECT.NO_OBJECT:
            hasObjectText.setText("none");
            break;

         case Environment.OBJECT.MOUSE:
            hasObjectText.setText("mouse");
            break;

         case Environment.OBJECT.STONE:
            hasObjectText.setText("stone");
            break;

         case Environment.OBJECT.EGG:
            hasObjectText.setText("egg");
            break;
         }

         // Response.
         if (bird.gender == Bird.MALE)
         {
            responseText.setText(MaleBird.RESPONSE.toString(bird.response));
         }
         else
         {
            responseText.setText(FemaleBird.RESPONSE.toString(bird.response));
         }
      }
   }

   // Driver panel.
   class DriverPanel extends JPanel
   {
      private static final long serialVersionUID = 0L;

      // Constructor.
      public DriverPanel()
      {
      }
   }
}
