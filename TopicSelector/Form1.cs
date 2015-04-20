using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TopicSelector
{
    public partial class TopicSelector : Form
    {
        List<string> m_Topics;
        List<string> m_AvailableTopics;
        Random m_Rand;

        public TopicSelector()
        {
            InitializeComponent();
        }

        private void TopicSelector_Load(object sender, EventArgs e)
        {
            m_Topics = new List<string>();
            m_AvailableTopics = new List<string>();
            m_Rand = new Random();

            string line;

            using (StreamReader file = new StreamReader("topics.txt"))
            {
                while ((line = file.ReadLine()) != null)
                {
                    m_Topics.Add(line);
                    m_AvailableTopics.Add(line);
                }

                file.Close();
            }
        }

        // New Topic
        private void button1_Click(object sender, EventArgs e)
        {
            if (m_AvailableTopics.Count == 0)
            {
                textBox1.Text = "No Topics left! Press Reset before picking a new topic.";
                return;
            }
                
            int index = m_Rand.Next(m_AvailableTopics.Count);

            textBox1.Text = m_AvailableTopics[index];
            m_AvailableTopics.RemoveAt(index);
        }

        // Reset Topics
        private void button2_Click(object sender, EventArgs e)
        {
            m_AvailableTopics.Clear();
            m_AvailableTopics.AddRange(m_Topics);
            textBox1.Text = "";
        }
    }
}
