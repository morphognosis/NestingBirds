// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting bird female replay dashboard.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import morphognosis.nestingbirds.Bird.ORIENTATION;

public class FemaleReplayDashboard extends JFrame
{
   private static final long serialVersionUID = 0L;
   
   // Replay state.
   String currentLocale = "PLAIN";
   String currentObject = "NO_OBJECT";
   String leftLocale = "PLAIN";
   String leftObject = "NO_OBJECT";
   String leftFrontLocale = "PLAIN";
   String leftFrontObject = "NO_OBJECT";
   String frontLocale = "PLAIN";
   String frontObject = "NO_OBJECT";
   String rightFrontLocale = "PLAIN";
   String rightFrontObject = "NO_OBJECT";
   String rightLocale = "PLAIN";
   String rightObject = "NO_OBJECT";
   String rightRearLocale = "PLAIN";
   String rightRearObject = "NO_OBJECT";
   String rearLocale = "PLAIN";
   String rearObject = "NO_OBJECT";
   String leftRearLocale = "PLAIN";
   String leftRearObject = "NO_OBJECT";
   String orientation = "NORTH";
   String goal = "LAY_EGG";
   String hasObject = "NO_OBJECT";
   String food = "0";
   String mouseNeed = "0.000000";
   String layEggNeed = "0.000000";
   String broodEggNeed = "0.000000";
   String response = "DO_NOTHING";
   
   // Components.
   StatusPanel statusPanel;

   // Constructor.
   public FemaleReplayDashboard()
   {
      setTitle("Female bird");
      addWindowListener(new WindowAdapter()
                        {
                           public void windowClosing(WindowEvent e) { close(); }
                        }
                        );
      JPanel basePanel = (JPanel)getContentPane();
      basePanel.setLayout(new BoxLayout(basePanel, BoxLayout.Y_AXIS));
      statusPanel = new StatusPanel();
      basePanel.add(statusPanel);
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
      statusPanel.update();
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
      JTextField currentLocaleText;
      JTextField currentObjectText;
      JTextField leftLocaleText;
      JTextField leftObjectText;
      JTextField leftFrontLocaleText;
      JTextField leftFrontObjectText;      
      JTextField frontLocaleText;
      JTextField frontObjectText;
      JTextField rightFrontLocaleText;
      JTextField rightFrontObjectText;      
      JTextField rightLocaleText;
      JTextField rightObjectText;
      JTextField rightRearLocaleText;
      JTextField rightRearObjectText;
      JTextField rearLocaleText;
      JTextField rearObjectText;
      JTextField leftRearLocaleText;
      JTextField leftRearObjectText;      
      JTextField orientationText;
      JTextField goalText;
      JTextField hasObjectText;      
      JTextField foodText;
      JTextField mouseNeedText;
      JTextField layEggNeedText;      
      JTextField broodEggNeedText;
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
         JPanel currentCellPanel = new JPanel();
         currentCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         currentCellPanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Current cell"));
         sensorsPanel.add(currentCellPanel);
         currentCellPanel.add(new JLabel("Locale:"));
         currentLocaleText = new JTextField(10);
         currentLocaleText.setEditable(false);
         currentCellPanel.add(currentLocaleText);
         currentCellPanel.add(new JLabel("Object:"));
         currentObjectText = new JTextField(10);
         currentObjectText.setEditable(false);
         currentCellPanel.add(currentObjectText);
         JPanel leftCellPanel = new JPanel();
         leftCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         leftCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Left cell"));
         sensorsPanel.add(leftCellPanel);
         leftCellPanel.add(new JLabel("Locale:"));
         leftLocaleText = new JTextField(10);
         leftLocaleText.setEditable(false);
         leftCellPanel.add(leftLocaleText);
         leftCellPanel.add(new JLabel("Object:"));
         leftObjectText = new JTextField(10);
         leftObjectText.setEditable(false);
         leftCellPanel.add(leftObjectText);
         JPanel leftFrontCellPanel = new JPanel();
         leftFrontCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         leftFrontCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Left front cell"));
         sensorsPanel.add(leftFrontCellPanel);
         leftFrontCellPanel.add(new JLabel("Locale:"));
         leftFrontLocaleText = new JTextField(10);
         leftFrontLocaleText.setEditable(false);
         leftFrontCellPanel.add(leftFrontLocaleText);
         leftFrontCellPanel.add(new JLabel("Object:"));
         leftFrontObjectText = new JTextField(10);
         leftFrontObjectText.setEditable(false);
         leftFrontCellPanel.add(leftFrontObjectText);         
         JPanel frontCellPanel = new JPanel();
         frontCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         frontCellPanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Front cell"));
         sensorsPanel.add(frontCellPanel);
         frontCellPanel.add(new JLabel("Locale:"));
         frontLocaleText = new JTextField(10);
         frontLocaleText.setEditable(false);
         frontCellPanel.add(frontLocaleText);
         frontCellPanel.add(new JLabel("Object:"));
         frontObjectText = new JTextField(10);
         frontObjectText.setEditable(false);
         frontCellPanel.add(frontObjectText);         
         JPanel rightFrontCellPanel = new JPanel();
         rightFrontCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         rightFrontCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Right front cell"));
         sensorsPanel.add(rightFrontCellPanel);
         rightFrontCellPanel.add(new JLabel("Locale:"));
         rightFrontLocaleText = new JTextField(10);
         rightFrontLocaleText.setEditable(false);
         rightFrontCellPanel.add(rightFrontLocaleText);
         rightFrontCellPanel.add(new JLabel("Object:"));
         rightFrontObjectText = new JTextField(10);
         rightFrontObjectText.setEditable(false);
         rightFrontCellPanel.add(rightFrontObjectText);             
         JPanel rightCellPanel = new JPanel();
         rightCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         rightCellPanel.setBorder(BorderFactory.createTitledBorder(
                                     BorderFactory.createLineBorder(Color.black),
                                     "Right cell"));
         sensorsPanel.add(rightCellPanel);
         rightCellPanel.add(new JLabel("Locale:"));
         rightLocaleText = new JTextField(10);
         rightLocaleText.setEditable(false);
         rightCellPanel.add(rightLocaleText);
         rightCellPanel.add(new JLabel("Object:"));
         rightObjectText = new JTextField(10);
         rightObjectText.setEditable(false);
         rightCellPanel.add(rightObjectText);
         JPanel rightRearCellPanel = new JPanel();
         rightRearCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         rightRearCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Right rear cell"));
         sensorsPanel.add(rightRearCellPanel);
         rightRearCellPanel.add(new JLabel("Locale:"));
         rightRearLocaleText = new JTextField(10);
         rightRearLocaleText.setEditable(false);
         rightRearCellPanel.add(rightRearLocaleText);
         rightRearCellPanel.add(new JLabel("Object:"));
         rightRearObjectText = new JTextField(10);
         rightRearObjectText.setEditable(false);
         rightRearCellPanel.add(rightRearObjectText);                     
         JPanel rearCellPanel = new JPanel();
         rearCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         rearCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Rear cell"));
         sensorsPanel.add(rearCellPanel);
         rearCellPanel.add(new JLabel("Locale:"));
         rearLocaleText = new JTextField(10);
         rearLocaleText.setEditable(false);
         rearCellPanel.add(rearLocaleText);
         rearCellPanel.add(new JLabel("Object:"));
         rearObjectText = new JTextField(10);
         rearObjectText.setEditable(false);
         rearCellPanel.add(rearObjectText);                  
         JPanel leftRearCellPanel = new JPanel();
         leftRearCellPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         leftRearCellPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Left rear cell"));
         sensorsPanel.add(leftRearCellPanel);
         leftRearCellPanel.add(new JLabel("Locale:"));
         leftRearLocaleText = new JTextField(10);
         leftRearLocaleText.setEditable(false);
         leftRearCellPanel.add(leftRearLocaleText);
         leftRearCellPanel.add(new JLabel("Object:"));
         leftRearObjectText = new JTextField(10);
         leftRearObjectText.setEditable(false);
         leftRearCellPanel.add(leftRearObjectText);            
         JPanel orientationPanel = new JPanel();
         orientationPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         orientationPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Orientation"));
         sensorsPanel.add(orientationPanel);
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         orientationPanel.add(orientationText);
         JPanel goalPanel = new JPanel();
         goalPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         goalPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Goal"));
         sensorsPanel.add(goalPanel);
         goalText = new JTextField(10);
         goalText.setEditable(false);
         goalPanel.add(goalText);
         JPanel hasObjectPanel = new JPanel();
         hasObjectPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         hasObjectPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Has object"));
         sensorsPanel.add(hasObjectPanel);
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         hasObjectPanel.add(hasObjectText);
         JPanel foodPanel = new JPanel();
         foodPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         foodPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Food"));
         sensorsPanel.add(foodPanel);
         foodText = new JTextField(10);
         foodText.setEditable(false);
         foodPanel.add(foodText);        
         JPanel needsPanel = new JPanel();
         needsPanel.setLayout(new BoxLayout(sensorsPanel, BoxLayout.Y_AXIS));
         needsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Needs"));
         add(needsPanel, BorderLayout.CENTER);
         JPanel mousePanel = new JPanel();
         mousePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         mousePanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Mouse"));
         needsPanel.add(mousePanel);
         mouseNeedText = new JTextField(10);
         mouseNeedText.setEditable(false);
         needsPanel.add(mouseNeedText);
         JPanel layEggPanel = new JPanel();
         layEggPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         layEggPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Lay egg"));
         needsPanel.add(layEggPanel);
         layEggNeedText = new JTextField(10);
         layEggNeedText.setEditable(false);
         layEggPanel.add(layEggNeedText);
         JPanel broodEggPanel = new JPanel();
         broodEggPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         broodEggPanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Brood egg"));
         needsPanel.add(leftFrontCellPanel);
         broodEggNeedText = new JTextField(10);
         broodEggNeedText.setEditable(false);
         broodEggPanel.add(broodEggNeedText);        
         JPanel responsePanel = new JPanel();
         responsePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         responsePanel.setBorder(BorderFactory.createTitledBorder(
                                    BorderFactory.createLineBorder(Color.black),
                                    "Response"));
         add(responsePanel, BorderLayout.SOUTH);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(25);
         responseText.setEditable(false);
         responsePanel.add(responseText);
      }


      // Update.
      public void update()
      {
         // Get sensor values.
         switch (bird.sensors[Bird.CURRENT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            currentLocaleText.setText(currentLocale);
            break;

         case NestingBirds.LOCALE.FOREST:
            currentLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            currentLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.CURRENT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            currentObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            currentObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            currentObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            currentObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.LEFT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            leftLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            leftLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            leftLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.LEFT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            leftObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            leftObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            leftObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            leftObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.FORWARD_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            forwardLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            forwardLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            forwardLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.FORWARD_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            forwardObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            forwardObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            forwardObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            forwardObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.RIGHT_LOCALE_SENSOR])
         {
         case NestingBirds.LOCALE.DESERT:
            rightLocaleText.setText("desert");
            break;

         case NestingBirds.LOCALE.FOREST:
            rightLocaleText.setText("forest");
            break;

         case NestingBirds.LOCALE.PLAIN:
            rightLocaleText.setText("plain");
            break;
         }
         switch (bird.sensors[Bird.RIGHT_OBJECT_SENSOR])
         {
         case NestingBirds.OBJECT.NO_OBJECT:
            rightObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            rightObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            rightObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            rightObjectText.setText("egg");
            break;
         }
         switch (bird.sensors[Bird.MATE_PROXIMITY_SENSOR])
         {
         case Bird.MATE_PROXIMITY_UNKNOWN:
            matePresentText.setText("unknown");
            break;

         case Bird.MATE_PROXIMITY_PRESENT:
            matePresentText.setText("present");
            break;

         case Bird.MATE_PROXIMITY_LEFT:
            matePresentText.setText("left");
            break;

         case Bird.MATE_PROXIMITY_FORWARD:
            matePresentText.setText("forward");
            break;

         case Bird.MATE_PROXIMITY_RIGHT:
            matePresentText.setText("right");
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
         case NestingBirds.OBJECT.NO_OBJECT:
            hasObjectText.setText("none");
            break;

         case NestingBirds.OBJECT.MOUSE:
            hasObjectText.setText("mouse");
            break;

         case NestingBirds.OBJECT.STONE:
            hasObjectText.setText("stone");
            break;

         case NestingBirds.OBJECT.EGG:
            hasObjectText.setText("egg");
            break;
         }

         // Response.
         responseText.setText(response);
      }
   }
}
