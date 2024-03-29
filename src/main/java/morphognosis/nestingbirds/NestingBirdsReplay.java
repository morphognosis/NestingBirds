// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds replay.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Label;
import java.awt.MediaTracker;
import java.awt.Point;
import java.awt.ScrollPane;
import java.awt.Scrollbar;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Objects;
import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.json.JSONException;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

public class NestingBirdsReplay extends JFrame implements Runnable, ActionListener, ChangeListener, MouseListener
{
   private static final long serialVersionUID = 0L;

   // Usage.
   public static final String Usage =
      "Usage: java morphognosis.nestingbirds.NestingBirdsReplay -behaviorFile <behavior file name>";

   // Behavior file name.
   static String BehaviorFilename;

   // Replay frames.
   ArrayList<NestingBirdsReplayFrame> replayFrames;

   // Nesting birds.
   NestingBirds nestingbirds;

   // Milliseconds between world updates.
   static final int MIN_RESPONSE_DELAY = 100;
   static final int MAX_RESPONSE_DELAY = 1000;

   // Milliseconds between display updates.
   static final int DISPLAY_UPDATE_DELAY = 50;

   // Dimensions.
   static final int       WIDTH              = 21;
   static final int       HEIGHT             = 21;
   static final Dimension SCREEN_SIZE        = new Dimension(652, 680);
   static final Dimension CANVAS_SIZE        = new Dimension(630, 630);
   static final Dimension CONTROL_PANEL_SIZE = new Dimension(652, 100);
   static final Dimension CELL_SIZE          = new Dimension(30, 30);
   static final Dimension LOCALE_SIZE        = new Dimension(15, 15);
   static final Dimension OBJECT_SIZE        = new Dimension(20, 20);
   static final Dimension SMALL_BIRD_SIZE    = new Dimension(15, 15);
   static final Dimension BIRD_SIZE          = new Dimension(80, 80);

   // Screen size.
   Dimension screenSize;

   // Display.
   Graphics   graphics;
   ScrollPane canvasScroll;
   Dimension  canvasScrollSize;
   Canvas     canvas;
   Graphics   canvasGraphics;
   Image      worldImage;
   Graphics   worldImageGraphics;
   Dimension  worldImageSize;
   Point      femaleStatusLocation;
   Point      femaleImageLocation;
   Point      femaleObjectLocation;
   Point      maleStatusLocation;
   Point      maleImageLocation;
   Point      maleObjectLocation;
   Point      statusInfoOffset;
   Point      localeImageOffset;
   Point      objectImageOffset;
   Point      femaleImageOffset;
   Point      maleImageOffset;
   String     message = "Click bird for dashboard";

   // Threads.
   Thread replayThread;
   Thread displayThread;

   // Control panel.
   JPanel    controlPanel;
   Dimension controlPanelSize;
   JButton   rewindButton;
   JSlider   speedSlider;
   int       speed;
   JButton   stepButton;
   JLabel    stepCounterLabel;
   int       stepCounter;

   // Images
   Image maleImage;
   Image maleSmallImage;
   Image femaleImage;
   Image femaleSmallImage;
   Image treeImage;
   Image cactusImage;
   Image mouseImage;
   Image stoneImage;
   Image eggImage;
   Image maleNetImage;
   Image femaleNetImage;

   // Font.
   Font        font = new Font(Font.MONOSPACED, Font.PLAIN, 12);
   FontMetrics fontMetrics;
   int         fontAscent;
   int         fontWidth;
   int         fontHeight;

   // Dashboards.
   MaleReplayDashboard   maleDashboard;
   FemaleReplayDashboard femaleDashboard;

   // Constructor.
   public NestingBirdsReplay()
   {
      // Load behavior file.
      loadReplayFile(BehaviorFilename);

      // Create dashboards.
      maleDashboard   = new MaleReplayDashboard();
      femaleDashboard = new FemaleReplayDashboard();

      // Create nesting birds.
      nestingbirds = new NestingBirds();

      // Set up display.
      setTitle("Nesting birds");
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      screenSize = SCREEN_SIZE;
      setSize(screenSize);
      setLayout(new BorderLayout());

      // Create control panel.
      controlPanel     = new JPanel();
      controlPanelSize = CONTROL_PANEL_SIZE;
      controlPanel.setBounds(0, 0, controlPanelSize.width, controlPanelSize.height);
      add(controlPanel, BorderLayout.NORTH);
      rewindButton = newButton("Rewind");
      rewindButton.addActionListener(this);
      controlPanel.add(rewindButton);
      controlPanel.add(newLabel("Speed: Fast", Label.RIGHT));
      speedSlider = newSlider(Scrollbar.HORIZONTAL, MIN_RESPONSE_DELAY,
                              MAX_RESPONSE_DELAY, MAX_RESPONSE_DELAY);
      speedSlider.addChangeListener(this);
      speed = MAX_RESPONSE_DELAY;
      controlPanel.add(speedSlider);
      controlPanel.add(newLabel("Stop", Label.LEFT));
      stepButton = newButton("Step");
      stepButton.addActionListener(this);
      controlPanel.add(stepButton);
      stepCounterLabel = newLabel("Step = 0/0");
      controlPanel.add(stepCounterLabel);
      stepCounter = 0;
      stepReplay();

      // Create world display.
      canvasScroll     = new ScrollPane();
      canvasScrollSize = new Dimension(screenSize.width, (int)((double)screenSize.height * .95));
      canvasScroll.setBounds(0, 0, canvasScrollSize.width, canvasScrollSize.height);
      canvas = new Canvas();
      canvas.setBounds(0, controlPanelSize.height, CANVAS_SIZE.width, CANVAS_SIZE.height + controlPanelSize.height);
      canvas.addMouseListener(this);
      add(canvasScroll, BorderLayout.SOUTH);
      canvasScroll.add(canvas, null);

      // Get images.
      MediaTracker tracker = new MediaTracker(this);
      Image        image   = loadImage("male.gif");
      tracker.addImage(image, 0);
      maleImage = image.getScaledInstance(BIRD_SIZE.width,
                                          BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleImage, 0);
      maleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                               SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleSmallImage, 0);
      image = loadImage("female.gif");
      tracker.addImage(image, 0);
      femaleImage = image.getScaledInstance(BIRD_SIZE.width,
                                            BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleImage, 0);
      femaleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                                 SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleSmallImage, 0);
      image = loadImage("tree.gif");
      tracker.addImage(image, 0);
      treeImage = image.getScaledInstance(LOCALE_SIZE.width,
                                          LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(treeImage, 0);
      image = loadImage("cactus.gif");
      tracker.addImage(image, 0);
      cactusImage = image.getScaledInstance(LOCALE_SIZE.width,
                                            LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(cactusImage, 0);
      image = loadImage("mouse.gif");
      tracker.addImage(image, 0);
      mouseImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(mouseImage, 0);
      image = loadImage("stone.gif");
      tracker.addImage(image, 0);
      stoneImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(stoneImage, 0);
      image = loadImage("egg.gif");
      tracker.addImage(image, 0);
      eggImage = image.getScaledInstance(OBJECT_SIZE.width,
                                         OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(eggImage, 0);
      try
      {
         tracker.waitForAll();
      }
      catch (InterruptedException e)
      {
         JOptionPane.showMessageDialog(this, "Image loading interrupted: " + e.getMessage());
         System.exit(1);
      }

      // Make display visible.
      pack();
      setLocation();
      setVisible(true);
   }


   // Make label with font.
   private JLabel newLabel(String text)
   {
      JLabel label = new JLabel(text);

      label.setFont(font);
      return(label);
   }


   // Make justified label with font.
   private JLabel newLabel(String text, int justification)
   {
      JLabel label = new JLabel(text, justification);

      label.setFont(font);
      return(label);
   }


   // Make button with font.
   private JButton newButton(String text)
   {
      JButton button = new JButton(text);

      button.setFont(font);
      return(button);
   }


   // Make slider with font.
   private JSlider newSlider(int orientation, int min, int max, int value)
   {
      JSlider slider = new JSlider(orientation, min, max, value);

      slider.setFont(font);
      return(slider);
   }


   public void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (dim.width - w) / 4;
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }


   // Start.
   public void start()
   {
      // Create replay update thread.
      if (replayThread == null)
      {
         replayThread = new Thread(this);
         replayThread.start();
      }

      // Create display update thread.
      if (displayThread == null)
      {
         displayThread = new Thread(this);
         displayThread.start();
      }
   }


   // Stop.
   public void stop()
   {
      if (replayThread != null)
      {
         replayThread = null;
      }

      if (displayThread != null)
      {
         displayThread = null;
      }
   }


   // Run.
   public void run()
   {
      // Lower this thread's priority and get the current time.
      Thread.currentThread().setPriority(Thread.MIN_PRIORITY);

      // Replay update loop.
      while (Thread.currentThread() == replayThread)
      {
         if (speed < MAX_RESPONSE_DELAY)
         {
            stepReplay();
         }

         // Set the timer for the next loop.
         try
         {
            Thread.sleep(speed);
         }
         catch (InterruptedException e) { break; }
      }

      // Display update loop.
      while (Thread.currentThread() == displayThread)
      {
         updateDisplay();

         try
         {
            Thread.sleep(DISPLAY_UPDATE_DELAY);
         }
         catch (InterruptedException e) { break; }
      }
   }


   // Step.
   public synchronized void stepReplay()
   {
      if (stepCounter < replayFrames.size())
      {
         stepCounterLabel.setText("Step = " + (stepCounter + 1) + "/" + replayFrames.size());
         NestingBirdsReplayFrame frame     = replayFrames.get(stepCounter);
         NestingBirdsReplayFrame framePrev = null;
         if (stepCounter > 0)
         {
            framePrev = replayFrames.get(stepCounter - 1);
         }
         nestingbirds.male.x   = frame.maleData.x;
         nestingbirds.male.y   = frame.maleData.y;
         nestingbirds.female.x = frame.femaleData.x;
         nestingbirds.female.y = frame.femaleData.y;
         femaleDashboard.update(frame);
         maleDashboard.update(frame);
         for (int x = 0; x < 8; x++)
         {
            for (int y = 0; y < 10; y++)
            {
               nestingbirds.world[x][y].object = NestingBirds.OBJECT.NO_OBJECT;
            }
         }
         for (int i = 0, j = frame.mice.size(); i < j; i++)
         {
            nestingbirds.world[frame.mice.get(i).x][frame.mice.get(i).y].object = NestingBirds.OBJECT.MOUSE;
         }
         if (framePrev != null)
         {
            if (framePrev.femaleData.response.equals("PUT_OBJECT") &&
                framePrev.femaleData.hasObject.equals("STONE") &&
                (nestingbirds.world[nestingbirds.female.x][nestingbirds.female.y].object == NestingBirds.OBJECT.NO_OBJECT))
            {
               nestingbirds.world[nestingbirds.female.x][nestingbirds.female.y].object = NestingBirds.OBJECT.STONE;
            }
            if (framePrev.femaleData.response.equals("LAY_EGG") &&
                (nestingbirds.world[nestingbirds.female.x][nestingbirds.female.y].object == NestingBirds.OBJECT.NO_OBJECT))
            {
               nestingbirds.world[nestingbirds.female.x][nestingbirds.female.y].object = NestingBirds.OBJECT.EGG;
            }
            if (framePrev.maleData.response.equals("GET_OBJECT") &&
                framePrev.maleData.hasObject.equals("NO_OBJECT") &&
                (nestingbirds.world[nestingbirds.male.x][nestingbirds.male.y].object == NestingBirds.OBJECT.STONE))
            {
               nestingbirds.world[nestingbirds.male.x][nestingbirds.male.y].object = NestingBirds.OBJECT.NO_OBJECT;
            }
         }
         if (frame.maleData.hasObject.equals("MOUSE"))
         {
            nestingbirds.male.hasObject = NestingBirds.OBJECT.MOUSE;
         }
         else if (frame.maleData.hasObject.equals("STONE"))
         {
            nestingbirds.male.hasObject = NestingBirds.OBJECT.STONE;
         }
         else
         {
            nestingbirds.male.hasObject = NestingBirds.OBJECT.NO_OBJECT;
         }
         if (frame.femaleData.hasObject.equals("MOUSE"))
         {
            nestingbirds.female.hasObject = NestingBirds.OBJECT.MOUSE;
         }
         else if (frame.femaleData.hasObject.equals("STONE"))
         {
            nestingbirds.female.hasObject = NestingBirds.OBJECT.STONE;
         }
         else
         {
            nestingbirds.female.hasObject = NestingBirds.OBJECT.NO_OBJECT;
         }
         stepCounter++;
      }
   }


   // Update display.
   public void updateDisplay()
   {
      int    x, y, x2, y2;
      String s;

      // Initialize graphics.
      if (graphics == null)
      {
         if (!initGraphics()) { return; }
      }

      // Clear.
      worldImageGraphics.setColor(Color.white);
      worldImageGraphics.fillRect(0, 0, worldImageSize.width, worldImageSize.height);

      // Draw world.
      worldImageGraphics.setColor(Color.black);
      y2 = worldImageSize.height;
      int h = CONTROL_PANEL_SIZE.height;
      for (x = x2 = 0; x < WIDTH; x++, x2 += CELL_SIZE.width)
      {
         worldImageGraphics.drawLine(x2, h, x2, y2);
      }
      worldImageGraphics.drawLine(x2, h, x2, y2);
      x2 = WIDTH * CELL_SIZE.width;
      for (y = 0, y2 = h; y < HEIGHT; y++, y2 += CELL_SIZE.height)
      {
         worldImageGraphics.drawLine(0, y2, x2, y2);
      }
      worldImageGraphics.drawLine(0, y2 - 1, x2, y2 - 1);
      for (x = x2 = 0; x < WIDTH; x++, x2 += CELL_SIZE.width)
      {
         for (y = 0, y2 = h; y < HEIGHT; y++, y2 += CELL_SIZE.height)
         {
            switch (nestingbirds.world[x][y].locale)
            {
            case NestingBirds.LOCALE.DESERT:
               worldImageGraphics.drawImage(cactusImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case NestingBirds.LOCALE.FOREST:
               worldImageGraphics.drawImage(treeImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case NestingBirds.LOCALE.PLAIN:
               break;
            }

            switch (nestingbirds.world[x][y].object)
            {
            case NestingBirds.OBJECT.NO_OBJECT:
               break;

            case NestingBirds.OBJECT.EGG:
               worldImageGraphics.drawImage(eggImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case NestingBirds.OBJECT.MOUSE:
               worldImageGraphics.drawImage(mouseImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case NestingBirds.OBJECT.STONE:
               worldImageGraphics.drawImage(stoneImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;
            }
         }
      }

      // Female status.
      worldImageGraphics.drawImage(femaleImage, femaleImageLocation.x,
                                   femaleImageLocation.y, this);
      s = "Female";
      x = femaleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = femaleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (nestingbirds.female.hasObject == NestingBirds.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      else if (nestingbirds.female.hasObject == NestingBirds.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      s = "Goal: " + femaleDashboard.replayFrame.femaleData.goal;
      x = femaleStatusLocation.x + statusInfoOffset.x;
      y = femaleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s  = "Response: " + femaleDashboard.replayFrame.femaleData.response;
      y += statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      x2 = nestingbirds.female.x * CELL_SIZE.width;
      y2 = (nestingbirds.female.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(femaleSmallImage, x2 + femaleImageOffset.x,
                                   y2 + femaleImageOffset.y, this);

      // Male status.
      worldImageGraphics.drawImage(maleImage, maleImageLocation.x,
                                   maleImageLocation.y, this);
      s = "Male";
      x = maleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = maleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (nestingbirds.male.hasObject == NestingBirds.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      else if (nestingbirds.male.hasObject == NestingBirds.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      s = "Goal: " + maleDashboard.replayFrame.maleData.goal;
      x = maleStatusLocation.x + statusInfoOffset.x;
      y = maleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s  = "Response: " + maleDashboard.replayFrame.maleData.response;
      y += statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      x2 = nestingbirds.male.x * CELL_SIZE.width;
      y2 = (nestingbirds.male.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(maleSmallImage, x2 + maleImageOffset.x,
                                   y2 + maleImageOffset.y, this);

      // Show message?
      if (message != null)
      {
         worldImageGraphics.setColor(Color.white);
         int w = fontMetrics.stringWidth(message);
         h = fontMetrics.getAscent() + fontMetrics.getDescent();
         worldImageGraphics.fillRect((worldImageSize.width - w) / 2, (worldImageSize.height / 6) - h, w, h);
         worldImageGraphics.setColor(Color.black);
         worldImageGraphics.drawString(message,
                                       (worldImageSize.width - w) / 2, worldImageSize.height / 6);
      }

      // Copy to display.
      canvasGraphics.drawImage(worldImage, 0, 0, this);
   }


   // Initialize graphics.
   boolean initGraphics()
   {
      int x, y;

      if (graphics != null) { return(true); }
      graphics = getGraphics();
      if (graphics == null) { return(false); }

      // Initialize font.
      graphics.setFont(font);
      fontMetrics = graphics.getFontMetrics();
      fontAscent  = fontMetrics.getMaxAscent();
      fontWidth   = fontMetrics.getMaxAdvance();
      fontHeight  = fontMetrics.getHeight();

      // Initialize graphics.
      if ((x = WIDTH * CELL_SIZE.width) < CONTROL_PANEL_SIZE.width)
      {
         x = CONTROL_PANEL_SIZE.width;
      }
      x++;
      y = (HEIGHT * CELL_SIZE.height) + CONTROL_PANEL_SIZE.height;
      worldImageSize     = new Dimension(x, y);
      worldImage         = createImage(x, y);
      worldImageGraphics = worldImage.getGraphics();
      worldImageGraphics.setFont(font);
      y = 1;
      femaleStatusLocation = new Point(0, y);
      x = (int)((CONTROL_PANEL_SIZE.width - (2.5 * BIRD_SIZE.width)) / 2);
      femaleImageLocation = new Point(x, y);
      x = femaleImageLocation.x + ((BIRD_SIZE.width * 7) / 8);
      y = femaleImageLocation.y + (BIRD_SIZE.height / 8);
      femaleObjectLocation = new Point(x, y);
      x = 1;
      y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
      femaleImageOffset = new Point(x, y);
      x = (int)(femaleImageLocation.x + (1.5 * BIRD_SIZE.width));
      y = 1;
      maleImageLocation = new Point(x, y);
      x = maleImageLocation.x + BIRD_SIZE.width;
      maleStatusLocation = new Point(x, y);
      x = maleImageLocation.x - (BIRD_SIZE.width / 8) - 1;
      y = maleImageLocation.y + (BIRD_SIZE.height / 8);
      maleObjectLocation = new Point(x, y);
      x = CELL_SIZE.width - SMALL_BIRD_SIZE.width;
      y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
      maleImageOffset = new Point(x, y);
      x = 2;
      y = 2 + fontAscent;
      statusInfoOffset  = new Point(x, y);
      localeImageOffset = new Point(1, 1);
      x = (CELL_SIZE.width - OBJECT_SIZE.width) / 2;
      y = (CELL_SIZE.height - OBJECT_SIZE.height) / 2;
      objectImageOffset = new Point(x, y);
      canvasGraphics    = canvas.getGraphics();
      canvasGraphics.setFont(font);
      return(true);
   }


   // Button listener.
   @Override
   public void actionPerformed(ActionEvent e)
   {
      if (e.getSource() == stepButton)
      {
         speed = MAX_RESPONSE_DELAY;
         speedSlider.setValue(MAX_RESPONSE_DELAY);
         stepReplay();
      }
      if (e.getSource() == rewindButton)
      {
         speed = MAX_RESPONSE_DELAY;
         speedSlider.setValue(MAX_RESPONSE_DELAY);
         nestingbirds = new NestingBirds();
         stepCounter  = 0;
         stepReplay();
      }
   }


   // Speed slider listener.
   @Override
   public void stateChanged(ChangeEvent e)
   {
      speed = speedSlider.getValue();
   }


   // Canvas mouse listener.
   @Override
   public void mouseClicked(MouseEvent e)
   {
      int x = e.getX();
      int y = e.getY();

      if ((x >= femaleImageLocation.x) && (x <= femaleImageLocation.x + BIRD_SIZE.width))
      {
         if ((y >= femaleImageLocation.y) && (y <= femaleImageLocation.y + BIRD_SIZE.height))
         {
            femaleDashboard.open();
            message = null;
         }
      }
      if ((x >= maleImageLocation.x) && (x <= maleImageLocation.x + BIRD_SIZE.width))
      {
         if ((y >= maleImageLocation.y) && (y <= maleImageLocation.y + BIRD_SIZE.height))
         {
            maleDashboard.open();
            message = null;
         }
      }
   }


   @Override
   public void mousePressed(MouseEvent e) {}


   @Override
   public void mouseReleased(MouseEvent e) {}


   @Override
   public void mouseEntered(MouseEvent e) {}


   @Override
   public void mouseExited(MouseEvent e) {}

   // Load image.
   Image loadImage(String name)
   {
      BufferedImage image    = null;
      String        protocol = this.getClass().getResource("").getProtocol();

      if (Objects.equals(protocol, "jar"))
      {
         try
         {
            image = ImageIO.read(getClass().getResource(name));
         }
         catch (Exception e)
         {
            JOptionPane.showMessageDialog(this, "Cannot load image for " + name + ": " + e.getMessage());
            System.exit(1);
         }
      }
      else
      {
         try
         {
            image = ImageIO.read(new File("res/images/" + name));
         }
         catch (Exception e)
         {
            JOptionPane.showMessageDialog(this, "Cannot load image for " + name + ": " + e.getMessage());
            System.exit(1);
         }
      }
      return(image);
   }


   // Load replay file into frames.
   void loadReplayFile(String filename)
   {
      replayFrames = new ArrayList<NestingBirdsReplayFrame>();

      try
      {
         FileReader reader = new FileReader(filename);
         JSONParser parser = new JSONParser();
         JSONArray  replay = (JSONArray)parser.parse(reader);
         for (Object o : replay)
         {
            NestingBirdsReplayFrame frame = new NestingBirdsReplayFrame();
            JSONObject              entry = (JSONObject)o;
            Long step = (Long)entry.get("Step");
            frame.step = step.intValue();
            JSONObject data = (JSONObject)entry.get("Data");
            JSONArray  mice = (JSONArray)data.get("Mice");
            for (Object m : mice)
            {
               JSONObject mouse = (JSONObject)m;
               Long       x     = (Long)mouse.get("x");
               Long       y     = (Long)mouse.get("y");
               NestingBirdsReplayFrame.Location location = frame.newLocation();
               location.x = x.intValue();
               location.y = y.intValue();
               frame.mice.add(location);
            }
            JSONObject female = (JSONObject)data.get("Female");
            NestingBirdsReplayFrame.FemaleData femaleData = frame.femaleData;
            JSONObject location = (JSONObject)female.get("Location");
            Long       x        = (Long)location.get("x");
            Long       y        = (Long)location.get("y");
            femaleData.x = x.intValue();
            femaleData.y = y.intValue();
            JSONObject sensors     = (JSONObject)female.get("Sensors");
            JSONObject cellSensors = (JSONObject)sensors.get("Cell sensors");
            femaleData.currentObject    = (String)cellSensors.get("Current");
            femaleData.leftObject       = (String)cellSensors.get("Left");
            femaleData.leftFrontObject  = (String)cellSensors.get("Left front");
            femaleData.frontObject      = (String)cellSensors.get("Front");
            femaleData.rightFrontObject = (String)cellSensors.get("Right front");
            femaleData.rightObject      = (String)cellSensors.get("Right");
            femaleData.rightRearObject  = (String)cellSensors.get("Right rear");
            femaleData.rearObject       = (String)cellSensors.get("Rear");
            femaleData.leftRearObject   = (String)cellSensors.get("Left rear");
            femaleData.orientation      = (String)sensors.get("Orientation");
            femaleData.goal             = (String)sensors.get("Goal");
            femaleData.hasObject        = (String)sensors.get("Has object");
            femaleData.food             = (Long)female.get("Food") + "";
            JSONObject needs = (JSONObject)female.get("Needs");
            femaleData.mouseNeed    = (Double)needs.get("Mouse") + "";
            femaleData.layEggNeed   = (Double)needs.get("Lay egg") + "";
            femaleData.broodEggNeed = (Double)needs.get("Brood egg") + "";
            femaleData.response     = (String)female.get("Response");
            JSONObject male = (JSONObject)data.get("Male");
            NestingBirdsReplayFrame.MaleData maleData = frame.maleData;
            location                  = (JSONObject)male.get("Location");
            x                         = (Long)location.get("x");
            y                         = (Long)location.get("y");
            maleData.x                = x.intValue();
            maleData.y                = y.intValue();
            sensors                   = (JSONObject)male.get("Sensors");
            maleData.locale           = (String)sensors.get("Locale");
            maleData.mouseProximity   = (String)sensors.get("Mouse proximity");
            maleData.stoneProximity   = (String)sensors.get("Stone proximity");
            maleData.femaleProximity  = (String)sensors.get("Female proximity");
            maleData.goal             = (String)sensors.get("Goal");
            maleData.hasObject        = (String)sensors.get("Has object");
            maleData.flying           = (Boolean)sensors.get("Flying") + "";
            maleData.femaleWantsMouse = (Boolean)sensors.get("Female wants mouse") + "";
            maleData.femaleWantsStone = (Boolean)sensors.get("Female wants stone") + "";
            maleData.food             = (Long)male.get("Food") + "";
            needs                     = (JSONObject)male.get("Needs");
            maleData.mouseNeed        = (Double)needs.get("Mouse") + "";
            maleData.femaleMouseNeed  = (Double)needs.get("Female mouse") + "";
            maleData.femaleStoneNeed  = (Double)needs.get("Female stone") + "";
            maleData.response         = (String)female.get("Response");
            maleData.attendFemaleNeed = (Double)needs.get("Attend female") + "";
            maleData.response         = (String)male.get("Response");
            replayFrames.add(frame);
         }
      }
      catch (JSONException e)
      {
         System.err.println("Cannot parse replay file " + filename + ":" + e.getMessage());
         System.exit(1);
      }
      catch (Exception e)
      {
         System.err.println("Cannot load replay file " + filename + ":" + e.getMessage());
         System.exit(1);
      }
   }


   // Main.
   public static void main(String[] args)
   {
      for (int i = 0; i < args.length; i++)
      {
         if (args[i].equals("-behaviorFile"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid behaviorFile option");
               System.err.println(Usage);
               System.exit(1);
            }
            BehaviorFilename = args[i];
            continue;
         }
         if (args[i].equals("-help") || args[i].equals("-h") || args[i].equals("-?"))
         {
            System.out.println(Usage);
            System.exit(0);
         }
         System.err.println(Usage);
         System.exit(1);
      }
      if (BehaviorFilename == null)
      {
         System.err.println(Usage);
         System.exit(1);
      }

      // Create display.
      NestingBirdsReplay nestingbirdsReplay = new NestingBirdsReplay();

      // Start.
      nestingbirdsReplay.start();
   }
}
