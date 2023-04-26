// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting bird female replay dashboard.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
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

   // Replay frame.
   NestingBirdsReplayFrame replayFrame;

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
      replayFrame = new NestingBirdsReplayFrame();
      statusPanel = new StatusPanel();
      basePanel.add(statusPanel);
      pack();
      setLocation();
      setVisible(false);
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
   void update(NestingBirdsReplayFrame replayFrame)
   {
      this.replayFrame = replayFrame;

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
      JTextField currentObjectText;
      JTextField leftObjectText;
      JTextField leftFrontObjectText;
      JTextField frontObjectText;
      JTextField rightFrontObjectText;
      JTextField rightObjectText;
      JTextField rightRearObjectText;
      JTextField rearObjectText;
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
         setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
         JPanel sensorsPanel = new JPanel();
         sensorsPanel.setLayout(new BoxLayout(sensorsPanel, BoxLayout.Y_AXIS));
         sensorsPanel.setBorder(BorderFactory.createTitledBorder(
                                   BorderFactory.createLineBorder(Color.black),
                                   "Sensors"));
         add(sensorsPanel);
         JPanel cellSensorsPanel = new JPanel();
         cellSensorsPanel.setLayout(new GridLayout(3, 3, 5, 5));
         cellSensorsPanel.setBorder(BorderFactory.createTitledBorder(
                                       BorderFactory.createLineBorder(Color.black),
                                       "Cells"));
         sensorsPanel.add(cellSensorsPanel);
         leftFrontObjectText = new JTextField(10);
         leftFrontObjectText.setEditable(false);
         cellSensorsPanel.add(leftFrontObjectText);
         frontObjectText = new JTextField(10);
         frontObjectText.setEditable(false);
         cellSensorsPanel.add(frontObjectText);
         rightFrontObjectText = new JTextField(10);
         rightFrontObjectText.setEditable(false);
         cellSensorsPanel.add(rightFrontObjectText);
         leftObjectText = new JTextField(10);
         leftObjectText.setEditable(false);
         cellSensorsPanel.add(leftObjectText);
         currentObjectText = new JTextField(10);
         currentObjectText.setEditable(false);
         cellSensorsPanel.add(currentObjectText);
         rightObjectText = new JTextField(10);
         rightObjectText.setEditable(false);
         cellSensorsPanel.add(rightObjectText);
         leftRearObjectText = new JTextField(10);
         leftRearObjectText.setEditable(false);
         cellSensorsPanel.add(leftRearObjectText);
         rearObjectText = new JTextField(10);
         rearObjectText.setEditable(false);
         cellSensorsPanel.add(rearObjectText);
         rightRearObjectText = new JTextField(10);
         rightRearObjectText.setEditable(false);
         cellSensorsPanel.add(rightRearObjectText);
         JPanel internalSensorsPanel = new JPanel();
         internalSensorsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(internalSensorsPanel);
         internalSensorsPanel.add(new JLabel("Orientation:"));
         orientationText = new JTextField(10);
         orientationText.setEditable(false);
         internalSensorsPanel.add(orientationText);
         internalSensorsPanel.add(new JLabel("Goal:"));
         goalText = new JTextField(15);
         goalText.setEditable(false);
         internalSensorsPanel.add(goalText);
         internalSensorsPanel.add(new JLabel("Has object:"));
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         internalSensorsPanel.add(hasObjectText);
         JPanel foodPanel = new JPanel();
         foodPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         foodPanel.setBorder(BorderFactory.createTitledBorder(
                                BorderFactory.createLineBorder(Color.black),
                                "Food"));
         add(foodPanel);
         foodText = new JTextField(10);
         foodText.setEditable(false);
         foodPanel.add(foodText);
         JPanel needsPanel = new JPanel();
         needsPanel.setLayout(new BoxLayout(needsPanel, BoxLayout.Y_AXIS));
         needsPanel.setBorder(BorderFactory.createTitledBorder(
                                 BorderFactory.createLineBorder(Color.black),
                                 "Needs"));
         add(needsPanel);
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
         add(responsePanel);
         responsePanel.add(new JLabel("Response:"));
         responseText = new JTextField(15);
         responseText.setEditable(false);
         responsePanel.add(responseText);
      }


      // Update.
      public void update()
      {
         currentObjectText.setText(replayFrame.femaleData.currentObject);
         leftObjectText.setText(replayFrame.femaleData.leftObject);
         leftFrontObjectText.setText(replayFrame.femaleData.leftFrontObject);
         frontObjectText.setText(replayFrame.femaleData.frontObject);
         rightFrontObjectText.setText(replayFrame.femaleData.rightFrontObject);
         rightObjectText.setText(replayFrame.femaleData.rightObject);
         rightRearObjectText.setText(replayFrame.femaleData.rightRearObject);
         rearObjectText.setText(replayFrame.femaleData.rearObject);
         leftRearObjectText.setText(replayFrame.femaleData.leftRearObject);
         orientationText.setText(replayFrame.femaleData.orientation);
         goalText.setText(replayFrame.femaleData.goal);
         hasObjectText.setText(replayFrame.femaleData.hasObject);
         foodText.setText(replayFrame.femaleData.food);
         mouseNeedText.setText(replayFrame.femaleData.mouseNeed);
         layEggNeedText.setText(replayFrame.femaleData.layEggNeed);
         broodEggNeedText.setText(replayFrame.femaleData.broodEggNeed);
         responseText.setText(replayFrame.femaleData.response);
      }
   }
}
