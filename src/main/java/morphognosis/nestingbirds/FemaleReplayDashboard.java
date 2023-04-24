// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting bird female replay dashboard.

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
         JPanel internalSensorsPanel = new JPanel();
         internalSensorsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(internalSensorsPanel);
         internalSensorsPanel.add(new JLabel("Orientation:"));
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         internalSensorsPanel.add(orientationText);
         internalSensorsPanel.add(new JLabel("Goal:"));
         goalText = new JTextField(10);
         goalText.setEditable(false);
         internalSensorsPanel.add(goalText);            
         internalSensorsPanel.add(new JLabel("Has object:"));
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         internalSensorsPanel.add(hasObjectText);
         internalSensorsPanel.add(new JLabel("Food:"));
         foodText = new JTextField(10);
         foodText.setEditable(false);
         internalSensorsPanel.add(foodText);           
         JPanel needsPanel = new JPanel();
         needsPanel.setLayout(new BoxLayout(needsPanel, BoxLayout.Y_AXIS));
         needsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Needs"));
         add(needsPanel, BorderLayout.CENTER);
         JPanel internalNeedsPanel = new JPanel();
         internalNeedsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         needsPanel.add(internalNeedsPanel);
         internalNeedsPanel.add(new JLabel("Mouse:"));
         mouseNeedText = new JTextField(10);
         mouseNeedText.setEditable(false);
         internalNeedsPanel.add(mouseNeedText);
         internalNeedsPanel.add(new JLabel("Lay egg:"));
         layEggNeedText = new JTextField(10);
         layEggNeedText.setEditable(false);
         internalNeedsPanel.add(layEggNeedText);            
         internalNeedsPanel.add(new JLabel("Brood egg:"));
         broodEggNeedText = new JTextField(10);
         broodEggNeedText.setEditable(false);
         internalNeedsPanel.add(broodEggNeedText);                     
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
    	   currentLocaleText.setText(currentLocale);
    	   currentObjectText.setText(currentObject);
    	   leftLocaleText.setText(leftLocale);
    	   leftObjectText.setText(leftObject);
    	   leftFrontLocaleText.setText(leftFrontLocale);
    	   leftFrontObjectText.setText(leftFrontObject);
    	   frontLocaleText.setText(frontLocale);
    	   frontObjectText.setText(frontObject);
    	   rightFrontLocaleText.setText(rightFrontLocale);
    	   rightFrontObjectText.setText(rightFrontObject);
    	   rightLocaleText.setText(rightLocale);
    	   rightObjectText.setText(rightObject);
    	   rightRearLocaleText.setText(rightRearLocale);
    	   rightRearObjectText.setText(rightRearObject);
    	   rearLocaleText.setText(rearLocale);
    	   rearObjectText.setText(rearObject);
    	   leftRearLocaleText.setText(leftRearLocale);
    	   leftRearObjectText.setText(leftRearObject);
    	   orientationText.setText(orientation);
    	   goalText.setText(goal);
    	   hasObjectText.setText(hasObject);
    	   foodText.setText(food);
    	   mouseNeedText.setText(mouseNeed);
    	   layEggNeedText.setText(layEggNeed);
    	   broodEggNeedText.setText(broodEggNeed);   	  
    	   responseText.setText(response);
      }
   }
}
