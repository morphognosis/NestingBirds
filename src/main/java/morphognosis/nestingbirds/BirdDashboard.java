// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting bird dashboard.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Choice;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

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
      JTextField hiveText;
      JTextField nectarText;
      JTextField nectarDanceDirectionText;
      JTextField nectarDanceDistanceText;
      JTextField orientationText;
      JTextField nectarCarryText;
      JTextField nectarDistanceDisplayText;
      JTextField responseText;

      // Constructor.
      public StatusPanel()
      {
         setLayout(new BorderLayout());
         setBorder(BorderFactory.createTitledBorder(
                      BorderFactory.createLineBorder(Color.black),
                      "State"));
         JPanel sensorsPanel = new JPanel();
         sensorsPanel.setLayout(new BoxLayout(sensorsPanel, BoxLayout.Y_AXIS));
         sensorsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Sensors"));
         add(sensorsPanel, BorderLayout.NORTH);
         JPanel hivePanel = new JPanel();
         hivePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(hivePanel);
         hivePanel.add(new JLabel("Hive presence:"));
         hiveText = new JTextField(10);
         hiveText.setEditable(false);
         hivePanel.add(hiveText);
         JPanel nectarPanel = new JPanel();
         nectarPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(nectarPanel);
         nectarPanel.add(new JLabel("Nectar presence:"));
         nectarText = new JTextField(10);
         nectarText.setEditable(false);
         nectarPanel.add(nectarText);
         JPanel nectarDanceDirectionPanel = new JPanel();
         nectarDanceDirectionPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(nectarDanceDirectionPanel);
         nectarDanceDirectionPanel.add(new JLabel("Nectar dance direction:"));
         nectarDanceDirectionText = new JTextField(10);
         nectarDanceDirectionText.setEditable(false);
         nectarDanceDirectionPanel.add(nectarDanceDirectionText);
         JPanel nectarDanceDistancePanel = new JPanel();
         nectarDanceDistancePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(nectarDanceDistancePanel);
         nectarDanceDistancePanel.add(new JLabel("Nectar dance distance:"));
         nectarDanceDistanceText = new JTextField(10);
         nectarDanceDistanceText.setEditable(false);
         nectarDanceDistancePanel.add(nectarDanceDistanceText);
         JPanel responsePanel = new JPanel();
         responsePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         add(responsePanel, BorderLayout.CENTER);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(25);
         responseText.setEditable(false);
         responsePanel.add(responseText);
         JPanel statePanel = new JPanel();
         statePanel.setLayout(new BoxLayout(statePanel, BoxLayout.Y_AXIS));
         statePanel.setBorder(BorderFactory.createTitledBorder(
                                 BorderFactory.createLineBorder(Color.black),
                                 "Internal"));
         add(statePanel, BorderLayout.SOUTH);
         JPanel orientationPanel = new JPanel();
         orientationPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         statePanel.add(orientationPanel);
         orientationPanel.add(new JLabel("Orientation: "));
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         orientationPanel.add(orientationText);
         JPanel nectarCarryPanel = new JPanel();
         nectarCarryPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         statePanel.add(nectarCarryPanel);
         nectarCarryPanel.add(new JLabel("Nectar carry: "));
         nectarCarryText = new JTextField(10);
         nectarCarryText.setEditable(false);
         nectarCarryPanel.add(nectarCarryText);
         JPanel nectarDistanceDisplayPanel = new JPanel();
         nectarDistanceDisplayPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         statePanel.add(nectarDistanceDisplayPanel);
         nectarDistanceDisplayPanel.add(new JLabel("Nectar distance display: "));
         nectarDistanceDisplayText = new JTextField(10);
         nectarDistanceDisplayText.setEditable(false);
         nectarDistanceDisplayPanel.add(nectarDistanceDisplayText);
      }


      // Update.
      public void update()
      {
         // Get sensor values.
         int[] sensors = bird.sensors;
         if (sensors[HoneyBee.HIVE_PRESENCE_INDEX] == 1.0f)
         {
            hiveText.setText("true");
         }
         else
         {
            hiveText.setText("false");
         }
         if (sensors[HoneyBee.NECTAR_PRESENCE_INDEX] == 1.0f)
         {
            nectarText.setText("true");
         }
         else
         {
            nectarText.setText("false");
         }
         if (sensors[HoneyBee.NECTAR_DANCE_DIRECTION_INDEX] >= 0.0f)
         {
            nectarDanceDirectionText.setText(Orientation.toName(
                                                (int)sensors[HoneyBee.NECTAR_DANCE_DIRECTION_INDEX]));
         }
         else
         {
            nectarDanceDirectionText.setText("NA");
         }
         if (sensors[HoneyBee.NECTAR_DANCE_DISTANCE_INDEX] == 0.0f)
         {
            nectarDanceDistanceText.setText("long");
         }
         else if (sensors[HoneyBee.NECTAR_DANCE_DISTANCE_INDEX] == 1.0f)
         {
            nectarDanceDistanceText.setText("short");
         }
         else
         {
            nectarDanceDistanceText.setText("NA");
         }

         // Update response.
         responseText.setText(HoneyBee.getResponseName(bee.response));

         // Update state.
         orientationText.setText(Orientation.toName(bee.orientation));
         nectarCarryText.setText(bee.nectarCarry + "");
         if (bee.nectarDistanceDisplay == -1)
         {
            nectarDistanceDisplayText.setText("NA");
         }
         else
         {
            nectarDistanceDisplayText.setText(bee.nectarDistanceDisplay + "");
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
