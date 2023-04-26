// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting bird male replay dashboard.

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

public class MaleReplayDashboard extends JFrame
{
   private static final long serialVersionUID = 0L;

   // Replay frame.
   NestingBirdsReplayFrame replayFrame;

   // Components.
   StatusPanel statusPanel;

   // Constructor.
   public MaleReplayDashboard()
   {
      setTitle("Male bird");
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
      JTextField localeText;
      JTextField mouseProximityText;
      JTextField stoneProximityText;
      JTextField femaleProximityText;
      JTextField goalText;
      JTextField hasObjectText;
      JTextField flyingText;
      JTextField foodText;
      JTextField femaleWantsMouseText;
      JTextField femaleWantsStoneText;
      JTextField mouseNeedText;
      JTextField femaleMouseNeedText;
      JTextField femaleStoneNeedText;
      JTextField attendFemaleNeedText;
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
         JPanel localePanel = new JPanel();
         localePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(localePanel);
         localePanel.add(new JLabel("Locale:"));
         localeText = new JTextField(10);
         localeText.setEditable(false);
         localePanel.add(localeText);
         JPanel proximityPanel = new JPanel();
         proximityPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         proximityPanel.setBorder(BorderFactory.createTitledBorder(
                                     BorderFactory.createLineBorder(Color.black),
                                     "Proximity"));
         sensorsPanel.add(proximityPanel);
         proximityPanel.add(new JLabel("Mouse:"));
         mouseProximityText = new JTextField(10);
         mouseProximityText.setEditable(false);
         proximityPanel.add(mouseProximityText);
         proximityPanel.add(new JLabel("Stone:"));
         stoneProximityText = new JTextField(10);
         stoneProximityText.setEditable(false);
         proximityPanel.add(stoneProximityText);
         proximityPanel.add(new JLabel("Female:"));
         femaleProximityText = new JTextField(10);
         femaleProximityText.setEditable(false);
         proximityPanel.add(femaleProximityText);
         JPanel internalSensorsPanel = new JPanel();
         internalSensorsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(internalSensorsPanel);
         internalSensorsPanel.add(new JLabel("Goal:"));
         goalText = new JTextField(15);
         goalText.setEditable(false);
         internalSensorsPanel.add(goalText);
         internalSensorsPanel.add(new JLabel("Has object:"));
         hasObjectText = new JTextField(10);
         hasObjectText.setEditable(false);
         internalSensorsPanel.add(hasObjectText);
         internalSensorsPanel.add(new JLabel("Flying:"));
         flyingText = new JTextField(10);
         flyingText.setEditable(false);
         internalSensorsPanel.add(flyingText);
         JPanel femaleSensorsPanel = new JPanel();
         femaleSensorsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
         sensorsPanel.add(femaleSensorsPanel);
         femaleSensorsPanel.add(new JLabel("Female wants mouse:"));
         femaleWantsMouseText = new JTextField(10);
         femaleWantsMouseText.setEditable(false);
         femaleSensorsPanel.add(femaleWantsMouseText);
         femaleSensorsPanel.add(new JLabel("Female wants stone:"));
         femaleWantsStoneText = new JTextField(10);
         femaleWantsStoneText.setEditable(false);
         femaleSensorsPanel.add(femaleWantsStoneText);
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
         needsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
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
         internalNeedsPanel.add(new JLabel("Female mouse:"));
         femaleMouseNeedText = new JTextField(10);
         femaleMouseNeedText.setEditable(false);
         internalNeedsPanel.add(femaleMouseNeedText);
         internalNeedsPanel.add(new JLabel("Female stone:"));
         femaleStoneNeedText = new JTextField(10);
         femaleStoneNeedText.setEditable(false);
         internalNeedsPanel.add(femaleStoneNeedText);
         internalNeedsPanel.add(new JLabel("Attend female:"));
         attendFemaleNeedText = new JTextField(10);
         attendFemaleNeedText.setEditable(false);
         internalNeedsPanel.add(attendFemaleNeedText);
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
         localeText.setText(replayFrame.maleData.locale);
         mouseProximityText.setText(replayFrame.maleData.mouseProximity);
         stoneProximityText.setText(replayFrame.maleData.stoneProximity);
         femaleProximityText.setText(replayFrame.maleData.femaleProximity);
         goalText.setText(replayFrame.maleData.goal);
         hasObjectText.setText(replayFrame.maleData.hasObject);
         flyingText.setText(replayFrame.maleData.flying);
         femaleWantsMouseText.setText(replayFrame.maleData.femaleWantsMouse);
         femaleWantsStoneText.setText(replayFrame.maleData.femaleWantsStone);
         foodText.setText(replayFrame.maleData.food);
         mouseNeedText.setText(replayFrame.maleData.mouseNeed);
         femaleMouseNeedText.setText(replayFrame.maleData.femaleMouseNeed);
         femaleStoneNeedText.setText(replayFrame.maleData.femaleStoneNeed);
         attendFemaleNeedText.setText(replayFrame.maleData.attendFemaleNeed);
         responseText.setText(replayFrame.maleData.response);
      }
   }
}
