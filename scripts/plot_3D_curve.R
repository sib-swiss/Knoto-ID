#!/usr/bin/env Rscript

## Copyright (C) 2017 by SIB Swiss Institute of Bioinformatics, Julien Dorier and Dimos Goundaroulis.
## 
## This file is part of project Knoto-ID.
## 
## Knoto-ID is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## (at your option) any later version.
## 
## Knoto-ID is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Knoto-ID.  If not, see <http://www.gnu.org/licenses/>.

library(optparse)

option_list = list(
    make_option(c("--output"), type = "character", default = "output.html",
                metavar="FILENAME",help = "output 3D curve to file FILENAME (webGL)."),
    make_option(c("-s", "--subchain"), type = "character", default = NULL,
              help = "plot subchain contained in FILENAME. If the file contains multiple subchains, plot only the first one.",metavar="FILENAME"),
    make_option(c("-c", "--cyclic"), action="store_true", default = FALSE,
                help = "input curve is cyclic."),
    make_option(c("-w","--curve-width"), type = "numeric", default = 1,
                help = "curve width multiplier.")
    )

description="
   Load 3D piecewise linear curve in xyz format from file FILENAME.

   Optionally load subchain informations (from file specified with --subchain).
   This file should be in the format produced by knotted_core program using
   argument --output. Note: only the first line will be taken into account.

   Output 3D curve in webGL format with select subchain highlighted in red. 

   Examples:
      plot_projection_map.R --output=output.html curve.xyz
      plot_projection_map.R --output=output.html --subchain=knotted_core.txt curve.xyz
      plot_projection_map.R --output=output.html --subchain=knotted_core.txt --cyclic curve.xyz

    
"
opt_parser = OptionParser(usage = "usage: %prog [options] FILENAME",option_list=option_list, description = description)
opt = parse_args(opt_parser,positional_arguments=1,convert_hyphens_to_underscores = TRUE)

thickness=0.01
output = opt$options$output
flag_cyclic = opt$options$cyclic
inputsubchain = opt$options$subchain
thickness = opt$options$curve_width*thickness
inputcurve = opt$arg
flag_plot_subchain=FALSE
if(!is.null(inputsubchain))
{
 subchain=read.table(inputsubchain,header=TRUE,comment.char="",sep="\t",check.names=FALSE)
 colnames(subchain)=gsub("#","",colnames(subchain))
 if(nrow(subchain)>0)
  {
      flag_plot_subchain=TRUE
      subchain.start=subchain[1,"index_first"]
      subchain.length=subchain[1,"length"]
  }
}


options(rgl.useNULL=TRUE)
library(rgl) #require pandoc


curve=read.table(inputcurve)
colnames(curve)=c("x","y","z")
npoints=nrow(curve)
##rescale & recenter
rangex=range(curve$x)
rangey=range(curve$y)
rangez=range(curve$z)
scale=max(rangex[2]-rangex[1],rangey[2]-rangey[1],rangez[2]-rangez[1])
centerx=(rangex[2]+rangex[1])/2
centery=(rangey[2]+rangey[1])/2
centerz=(rangez[2]+rangez[1])/2
curve$x=(curve$x-centerx)/(scale)
curve$y=(curve$y-centery)/(scale)
curve$z=(curve$z-centerz)/(scale)

##extract subchain. NOTE: subchain.start is 0-based indexing. In R we use 1-based indexing
if(flag_plot_subchain)
{
ind=(subchain.start+(1:subchain.length)-1)%%nrow(curve)+1
subchain=curve[ind,]
}

##plot base curve
overlap=0
if(flag_cyclic)
{
  curve=rbind(curve,curve[1,])
  overlap=1
}
curve.col="red"
if(flag_plot_subchain)
{
    curve.col="white"
}
##eval tangent
e1=curve
e1=e1[-1,]-e1[-nrow(e1),]
##duplicate points
curve=rbind(curve[1,],curve[rep(2:(nrow(curve)-1),each=2),] ,curve[nrow(curve),])
e1=e1[rep(1:nrow(e1),each=2),]
##perturb duplicate points (otherwise cylinder3d fails)
curve[seq(2,nrow(curve)-1,by=2),]=curve[seq(2,nrow(curve)-1,by=2),]-0.001*e1[seq(2,nrow(curve)-1,by=2),]
##plot
shade3d((cylinder3d(curve,radius=thickness,sides=12,closed=overlap,e1=e1)), col =curve.col)
if(!flag_cyclic)
{
    #add endpoints
    spheres3d(curve[c(1,nrow(curve)),],radius=1.1*thickness, col =curve.col)
}


##plot subchain
if(flag_plot_subchain)
{
    ##eval tangent
    e1=subchain
    e1=e1[-1,]-e1[-nrow(e1),]
    ##duplicate points
    subchain=rbind(subchain[1,],subchain[rep(2:(nrow(subchain)-1),each=2),] ,subchain[nrow(subchain),])
    e1=e1[rep(1:nrow(e1),each=2),]
    ##perturb duplicate points (otherwise cylinder3d fails)
    subchain[seq(2,nrow(subchain)-1,by=2),]=subchain[seq(2,nrow(subchain)-1,by=2),]-0.001*e1[seq(2,nrow(subchain)-1,by=2),]
    #plot
    shade3d((cylinder3d(subchain,radius=1.1*thickness,sides=12,closed=0,e1=e1)), col ="red")
    #add endpoints
    spheres3d(subchain[c(1,nrow(subchain)),],radius=1.2*thickness, col ="red")
}
cat("creating  ",output,"\n")
##NOTE: selfcontained=TRUE -> require pandoc.
tryCatch(
    htmlwidgets::saveWidget(rglwidget(sizingPolicy=htmlwidgets::sizingPolicy(padding = 0, browser.fill = TRUE,viewer.fill=TRUE)), output),
    error=function(e){
        cat("***************************************************\n")
        cat("WARNING: could not create self contained html file\n")
        cat(output,"\n")               
        cat("External resources are saved in directory\n")
        cat(paste(tools::file_path_sans_ext(basename(output)),"_files", sep = ""),"/ \n",sep="")
        cat("To avoid this message, install pandoc (http://pandoc.org/)\n")
        cat("and make sure it can be found by R.\n")
        cat("***************************************************\n")
        htmlwidgets::saveWidget(rglwidget(sizingPolicy=htmlwidgets::sizingPolicy(padding = 0, browser.fill = TRUE,viewer.fill=TRUE)), output,selfcontained=FALSE)
       }
    )

