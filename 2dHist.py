import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.colors import LinearSegmentedColormap

from matplotlib import font_manager
font_manager.get_font_names()

plt.rcParams['font.family'] = 'Liberation Serif'
plt.rcParams['font.size'] = 15

colors = [
    (0.0, "white"),    # Start with white
    (0.2, "blue"),     # Transition to blue
    (0.4, "green"),    # Transition to green
    (0.6, "yellow"),   # Transition to yellow
    (0.8, "orange"),   # Transition to orange
    (1.0, "red"),      # End with red
]

root_cmap = LinearSegmentedColormap.from_list("ROOT", colors)

df_DT_Test = pd.read_csv("Outputs/C13_an_JENDL_ThinTarg.csv", delimiter=";", names = ["Energy","Position","Momentum","ParticleID","Weight"])
df_DT_Test_2 = pd.read_csv("Outputs/C13_an_TENDL_ThinTarg.csv", delimiter=";", names = ["Energy","Position","Momentum","ParticleID","Weight"])
fancy_DT_Test = pd.read_csv("Outputs/C13_an_Fancy_JENDL_ThinTarg_26.csv", delimiter=";", names = ["Energy","Position","Momentum","ParticleID","Weight"])

nbins = [100,30]
JENDL_e_a_hist,yedges1,xedges1 = np.histogram2d(df_DT_Test['Energy'],np.cos(df_DT_Test['Momentum']),bins=nbins,range=[[0,5000],[-1.1,1.1]])#,weights=0.1/(np.sin(df_DT_Test["Momentum"])**2))
X,Y = np.meshgrid(yedges1[:-1],xedges1[:-1])
Fancy_e_a_hist,yedges,xedges = np.histogram2d(fancy_DT_Test['Energy'],np.cos(fancy_DT_Test['Momentum']),bins=nbins,range=[[3000,5000],[-1.1,1.1]])#,weights=0.1/np.sin(fancy_DT_Test["Momentum"])**2)

X1,Y1 = np.meshgrid(yedges[:-1],xedges[:-1])
JENDL_e_a_hist = JENDL_e_a_hist.transpose()
Fancy_e_a_hist = Fancy_e_a_hist.transpose()

fig1,ax1 = plt.subplots(subplot_kw={'projection':'3d'})
fig1.set_size_inches(5,5)
#ax2.plot_surface(X,Y,JENDL_e_a_hist,cmap = 'coolwarm')
ax1.set_xlabel("Neutron Energy [keV]")
ax1.set_ylabel("Cos($\Theta$) Emission Angle")
ax1.set_zlabel("Counts")
ax1.plot_surface(X,Y,JENDL_e_a_hist,cmap = 'Spectral_r')

#plt.figure()
#plt.hist(np.cos(fancy_DT_Test['Momentum']),bins=100)

fig,ax2 = plt.subplots(subplot_kw={'projection':'3d'})
fig.set_size_inches(5,5)
#ax2.plot_surface(X,Y,JENDL_e_a_hist,cmap = 'coolwarm')
ax2.set_xlabel("Neutron Energy [keV]")
ax2.set_ylabel("Cos($\Theta$) Emission Angle")
ax2.set_zlabel("Counts")
ax2.plot_surface(X1,Y1,Fancy_e_a_hist,cmap = 'Spectral_r')
#ax2.set_zticks([])
#fig.set_layout_engine('tight')

plt.show()