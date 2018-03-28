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
library(ggplot2)

option_list = list(
    make_option(c("-o", "--output"), type = "character", default = "output.pdf",
                help = "output filename (with extension png, jpg, tiff, pdf or svg)."),
    make_option(c("-l", "--labels"), action="store_true", default = FALSE,
                help = "show labels."),
    make_option(c("-c", "--circles"), action="store_true", default = FALSE,
                help = "if given in input file, plot circles."),
    make_option(c("-e", "--edges"), action="store_true", default = FALSE,
                help = "if given in input file, plot graph edges."),
    make_option(c("-w","--line-width"), type = "numeric", default = 1,
                help = "line width multiplier."),
    make_option(c("-z","--zoom"), action = "store_true", default = FALSE,
                help = "Ignore circles and graph edges to evaluate plotting region.")
    )

description="
   Plot diagram contained in FILENAME.
   All files should be in the format produced by convert_diagram using
   --output-format=xyz, xyz+, xyz0 or xyz0+

   Examples:
      scripts/plot_diagram.R --output=output.png  diagram.txt
      scripts/plot_diagram.R --output=output.png  --labels diagram.txt
      scripts/plot_diagram.R --output=output.png  --circles --edges diagram.txt

    
"
opt_parser = OptionParser(usage = "usage: %prog [options] FILENAME",option_list=option_list, description = description)
opt = parse_args(opt_parser,positional_arguments=1,convert_hyphens_to_underscores = TRUE)

output = opt$options$output
flag_circles = opt$options$circles
flag_segments = opt$options$edges
flag_labels = opt$options$labels
flag_zoom = opt$options$zoom
inputfile = opt$arg
line_width = opt$options$line_width

ext_radius=1


data=read.table(inputfile,header=TRUE,comment.char="",stringsAsFactors=FALSE)
colnames(data)[1]="x"

data.circles=subset(data,grepl("^Circle\\|",data$label))
data.segments=subset(data,grepl("^Segment\\|",data$label))
data.curve=subset(data,(!grepl("^Segment\\|",data$label))&(!grepl("^Circle\\|",data$label)))


if(grepl(".png$",output))
    {
        png(output,2000,2000,res=200)
    }
if(grepl(".jpg|.jpeg$",output))
    {
        jpeg(output,2000,2000,res=200)
    }
if(grepl(".tiff|.tif$",output))
    {
        tiff(output,2000,2000,res=200)
    }
if(grepl(".pdf$",output))
    {
        pdf(output,10,10)
    }
if(grepl(".svg$",output))
    {
        svg(output,10,10)
    }
cat("Creating",output,"\n")

p=ggplot()
if(flag_circles&&nrow(data.circles)>0)
    {        
        data.circles$is.exterior=ifelse(gsub(".*is.exterior=(TRUE|FALSE).*","\\1",data.circles$label)=="TRUE","External","Internal")
        data.circles$is.dummy=ifelse(grepl("dummy",data.circles$label),"dummy","regular")
        p=p+geom_polygon(data=data.circles,aes(x, y, group=label,fill=is.dummy),alpha=0.2, colour="grey",size=line_width*0.3)
    }
if(flag_segments&&nrow(data.segments)>0)
    {
        p=p+geom_path(data=data.segments,aes(x, y, group=label),alpha=0.2, colour="grey",size=line_width*0.2)
    }
if(flag_circles&&nrow(data.circles)>0)
    {        
        data.circles.labels=aggregate(cbind(x,y,z)~label,data.circles,mean)
        data.circles.labels$type=gsub("Circle\\|([^_]*)_.*","\\1",data.circles.labels$label)
        data.circles.labels$label=gsub("Circle\\|([^_|]*_[^_|]*).*","\\1",data.circles.labels$label)
        if(flag_labels)
            {
                p=p+geom_text(data=data.circles.labels, aes(x, y, label=label),size=line_width*1,color="grey")
            }
    }

#p=p+geom_path(data=data.curve, aes(x, y), colour="white",size=line_width*4) 
p=p+geom_path(data=data.curve, aes(x, y), lineend = "round", colour="black",size=line_width*1) 
p=p+geom_path(data=subset(data.curve,z>=0),lineend = "butt", aes(x, y,group=label), colour="white",size=line_width*4) 
p=p+geom_path(data=subset(data.curve,z>=0), lineend = "round", aes(x, y,group=label), colour="black",size=line_width*1) 

if(flag_labels&&!flag_circles)
    {
        data.curve.labels=aggregate(cbind(x,y,z)~label,data.curve,function(x){(x[floor((length(x)-1)/2)+1]+x[ceiling((length(x)-1)/2)+1])/2.0})
        data.curve.labels=subset(data.curve.labels,grepl("^Arc",label))
        data.curve.labels$label=gsub("Arc_(.*)","arc \\1",data.curve.labels$label)
        if(flag_labels)
            {
                p=p+geom_label(data=data.curve.labels, aes(x, y, label=label),colour="blue",size=line_width*4,alpha=0.95,vjust=0.5,hjust=0.5,label.padding= unit(0.1, "lines"))
            }
    }
{
if(flag_zoom)
    {
        p=p+coord_equal(xlim=range(data.curve$x),ylim=range(data.curve$y))
    }
else
    {
        p=p+coord_equal()
    }
}
p=p+theme_bw()
p=p+scale_color_discrete("")
#p=p+scale_fill_manual("",values= c("External"="grey","Internal"="white"),guide="none")
p=p+scale_fill_manual("",values= c("regular"="grey","dummy"="white"),guide="none")
p=p+theme(axis.text=element_blank(),axis.title=element_blank(),axis.ticks=element_blank(),axis.line=element_blank(),panel.grid=element_blank(),panel.border=element_blank())
print(p)


dev.off()
