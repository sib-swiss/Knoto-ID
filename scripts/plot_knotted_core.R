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
library(RColorBrewer) 
library(reshape2)

option_list = list(
  make_option(c("-o", "--output"), type = "character", default = "output.png",
              help = "output filename (png format)."),
  make_option(c("-c", "--cyclic"), action="store_true", default = FALSE,
              help = "output disk matrix (polar coordinates: r=subchain length, angle=subchain middle point). Otherwise output fingerprint matrix (cartesian coordinates: abscissa=subchain start index, ordinate=subchain end index)."),
  make_option(c("-s", "--search-path"), type = "character", default = NA,
              help = "overlay search path contained in FILENAME with consecutive points connected by blue lines. Draw a black border around all subchains appearing in FILENAME",metavar="FILENAME"),
  make_option(c("-k", "--knotted-core"), type = "character", default = NA,
              help = "plot knotted core(s) contained in FILENAME with yellow circle(s).",metavar="FILENAME"),
  make_option(c("", "--no-transparency"), action="store_true", default = FALSE,
              help = "ignore frequency of the dominant knot(oid) type."),
  make_option(c("-b", "--boundaries"), action="store_true", default = FALSE,
              help = "draw white lines to show boundaries of domains with same knot(oid) type.")
    )

description="
   Plot data contained in FILENAME in the form of a heatmap, optionally
   overlaying search path (--search-path) and knotted-core (--knotted-core).
   All files should be in the format produced by knotted_core program using
   arguments --output=knotted_core.txt --output-search=search_path.txt --output-all=all.txt 

   Examples:
      plot_knotted_core.R --output=output.png all.txt
      plot_knotted_core.R --output=output.png --search-path=search_path.txt all.txt
      plot_knotted_core.R --output=output.png --search-path=search_path.txt --knotted-core=knotted_core.txt all.txt
      plot_knotted_core.R --output=output.png --knotted-core=knotted_core.txt search_path.txt
      plot_knotted_core.R --cyclic --output=output.png --search-path=search_path.txt --knotted-core=knotted_core.txt all.txt


    
"
opt_parser = OptionParser(usage = "usage: %prog [options] FILENAME",option_list=option_list, description = description)
opt = parse_args(opt_parser,positional_arguments=1,convert_hyphens_to_underscores = TRUE)

output = opt$options$output
inputmap = opt$arg
inputpath = opt$options$search_path
inputpoint = opt$options$knotted_core
flag_cyclic = opt$options$cyclic
flag_no_transparency = opt$options$no_transparency
flag_boundaries = opt$options$boundaries


data=read.table(inputmap, sep="\t", header = T,comment.char="")
if(ncol(data)==6)
    {
        colnames(data)=c("start.index","end.index","length","frequency","type","polynomial")
        types=unique(as.character(data$type))
        types=types[order(gsub("([^0-9]*)([0-9]*)([^0-9]*)([0-9]*).*","\\1",types),
            grepl("\\*",types),
            as.numeric(gsub("([^0-9]*)([0-9]*)([^0-9]*)([0-9]*)(.*)","\\2",types)),
            gsub("([^0-9]*)([0-9]*)([^0-9]*)([0-9]*)(.*)","\\3",types),
            as.numeric(gsub("([^0-9]*)([0-9]*)([^0-9]*)([0-9]*)(.*)","\\4",types)),
            gsub("([^0-9]*)([0-9]*)([^0-9]*)([0-9]*)(.*)","\\5",types))]
        data$type=factor(data$type,levels=types)
    }
if(ncol(data)==5)
    {
        colnames(data)=c("start.index","end.index","length","frequency","polynomial")
        data$type=data$polynomial
        data$type=factor(data$type,levels=sort(unique(as.character(data$type))))
    }
if(flag_no_transparency)
    {
        data$frequency=1
    }
N=max(data$start.index+1,data$end.index+1,na.rm=TRUE)
cat("N=",N,"\n")
data$middle=(data$start.index+data$length/2)%%N
data$middle_width=1
data$theta=(data$middle/N)*2
data$theta_width=2/N

if(!is.na(inputpath))
    {
        data_path=read.table(inputpath, sep="\t", header = T,comment.char="")
        if(ncol(data_path)==6)
            {
                colnames(data_path)=c("start.index","end.index","length","frequency","type","polynomial")
            }
        if(ncol(data_path)==5)
            {
                colnames(data_path)=c("start.index","end.index","length","frequency","polynomial")
                data_path$type=data_path$polynomial
            }
        if(flag_no_transparency)
            {
                data_path$frequency=1
            }        
        data_path$middle=(data_path$start.index+data_path$length/2)%%N
        ##contours:
        data_path_contours=data_path
        data_path_contours$middle_width=1
        ##line:
        data_path_line=subset(data_path,polynomial==data_path$polynomial[1])
    }
if(!is.na(inputpoint))
    {
        data_point=read.table(inputpoint, sep="\t", header = T,comment.char="")
        if(ncol(data_point)==6)
            {
                colnames(data_point)=c("start.index","end.index","length","frequency","type","polynomial")
            }
        if(ncol(data_point)==5)
            {
                colnames(data_point)=c("start.index","end.index","length","frequency","polynomial")
                data_point$type=data_point$polynomial
            }
        if(flag_no_transparency)
            {
                data_point$frequency=1
            }

        data_point$middle=(data_point$start.index+data_point$length/2)%%N
        data_point$middle_width=1
        data_point$theta=(data_point$middle/N)*2
        
    }


if(flag_cyclic)
    {

        if(flag_boundaries)
            {
                data.mat=acast(unique(data[,c("start.index","length","polynomial")]),start.index~length,value.var="polynomial",fill=NA)
                data_boundaries=NULL
                ##difference start.index vs start.index+1
                ind1=c(2:nrow(data.mat),1)
                tmp=data.frame(which(matrix(data.mat!=data.mat[ind1,],nrow=nrow(data.mat),ncol=ncol(data.mat)),arr.ind=TRUE))
                tmp$start.index=as.numeric(rownames(data.mat)[tmp$row])
                tmp$length=as.numeric(colnames(data.mat)[tmp$col])
                tmp$start.index=tmp$start.index+0.5
                tmp$middle=(tmp$start.index+tmp$length/2)%%N                
                tmp$middle1=tmp$middle
                tmp$middle2=tmp$middle
                tmp$length1=tmp$length-0.5
                tmp$length2=tmp$length+0.5                
                data_boundaries=unique(rbind(data_boundaries,tmp[,c("middle1","middle2","length1","length2")]))

                ##difference (length,start.index) vs (length+1,start.index)
                ind2=c(2:ncol(data.mat),NA)
                tmp=data.frame(which(matrix(!is.na(data.mat)&!is.na(data.mat[,ind2])&data.mat!=data.mat[,ind2],nrow=nrow(data.mat),ncol=ncol(data.mat)),arr.ind=TRUE))
                tmp$start.index=as.numeric(rownames(data.mat)[tmp$row])
                tmp$length=as.numeric(colnames(data.mat)[tmp$col])
                tmp$middle=(tmp$start.index+tmp$length/2)%%N                
                tmp$length=tmp$length+0.5
                tmp$middle1=tmp$middle
                tmp$middle2=tmp$middle+0.5
                tmp[tmp$middle2>N,c("middle1","middle2")]=tmp[tmp$middle2>N,c("middle1","middle2")]-N
                
                tmp$length1=tmp$length
                tmp$length2=tmp$length
                data_boundaries=unique(rbind(data_boundaries,tmp[,c("middle1","middle2","length1","length2")]))
                ##difference (length,start.index) vs (length+1,start.index-1)
                ind1=c(nrow(data.mat),1:(nrow(data.mat)-1))
                ind2=c(2:ncol(data.mat),NA)
                tmp=data.frame(which(matrix(!is.na(data.mat)&!is.na(data.mat[ind1,ind2])&data.mat!=data.mat[ind1,ind2],nrow=nrow(data.mat),ncol=ncol(data.mat)),arr.ind=TRUE))
                tmp$start.index=as.numeric(rownames(data.mat)[tmp$row])
                tmp$length=as.numeric(colnames(data.mat)[tmp$col])
                tmp$middle=(tmp$start.index+tmp$length/2)%%N                
                tmp$length=tmp$length+0.5
                tmp$middle1=tmp$middle-0.5
                tmp$middle2=tmp$middle
                tmp[tmp$middle1<0,c("middle1","middle2")]=tmp[tmp$middle1<0,c("middle1","middle2")]+N
                tmp$length1=tmp$length
                tmp$length2=tmp$length
                data_boundaries=unique(rbind(data_boundaries,tmp[,c("middle1","middle2","length1","length2")]))
            }
        
        ##split entries with theta=0
        datatmp=data
        datatmp[!is.na(datatmp$middle)&datatmp$middle<1e-10,"middle_width"]=datatmp[!is.na(datatmp$middle)&datatmp$middle<1e-10,"middle_width"]/2
        datatmp2=datatmp[!is.na(datatmp$middle)&datatmp$middle<1e-10,]
        datatmp[!is.na(datatmp$middle)&datatmp$middle<1e-10,"middle"]=datatmp[!is.na(datatmp$middle)&datatmp$middle<1e-10,"middle_width"]/2
        datatmp2$middle=N-datatmp2$middle_width/2
        datatmp=rbind(datatmp,datatmp2)

        if(!is.na(inputpath))
            {
                ##add missing contours (around middle=0), to plot with geom_path
                if(any(!is.na(data_path_contours$middle)&data_path_contours$middle<1e-10))
                    {
                        data_path_contours_zero=data_path_contours[!is.na(data_path_contours$middle)&data_path_contours$middle<1e-10,c("length","middle_width")]
                        data_path_contours_zero$index=1:nrow(data_path_contours_zero)
                        data_path_contours_missing=do.call("rbind",apply(data_path_contours_zero,1,function(x){
                            data.frame(middle=c(0,0.5,0.5,0,N,N-0.5,N-0.5,N),length=c(x["length"]-0.5,x["length"]-0.5,x["length"]+0.5,x["length"]+0.5,x["length"]-0.5,x["length"]-0.5,x["length"]+0.5,x["length"]+0.5),group=c(2*x["index"],2*x["index"],2*x["index"],2*x["index"],2*x["index"]+1,2*x["index"]+1,2*x["index"]+1,2*x["index"]+1))
                        }))
                    }
                else
                    {
                        data_path_contours_missing=NULL
                    }

                ##add group
                data_path_line$group=(data_path_line$middle/N)*2
                data_path_line$group=cumsum(round(c(0,abs(data_path_line$group[-nrow(data_path_line)]-data_path_line$group[-1]))))/2        
                ##split lines that cross middle=0
                to_duplicate=c((data_path_line$group[-nrow(data_path_line)]!=data_path_line$group[-1]))
                if(any(to_duplicate))
                    {
                        to_duplicate1=c(to_duplicate,FALSE)&!is.na(data_path_line$middle)&data_path_line$middle<1e-10
                        to_duplicate2=c(FALSE,to_duplicate)&!is.na(data_path_line$middle)&data_path_line$middle<1e-10
                        
                        data_path_line$index=1:nrow(data_path_line)
                        tmp1=NULL
                        tmp2=NULL
                        if(any(to_duplicate1))
                           {
                               tmp1=data_path_line[to_duplicate1,]
                               tmp1$index=tmp1$index+0.25
                               tmp1$group=tmp1$group+1
                               tmp1$middle=N
                           }
                        if(any(to_duplicate2))
                           {
                               tmp2=data_path_line[to_duplicate2,]
                               tmp2$index=tmp2$index-0.25
                               tmp2$group=tmp2$group-1
                               tmp2$middle=N
                           }
                        data_path_line=rbind(data_path_line,tmp1)
                        data_path_line=rbind(data_path_line,tmp2)
                        data_path_line=data_path_line[order(data_path_line$index),]
                    }                
            }

        png(output,3000,2000,res=300)
        p=ggplot(data=datatmp,aes(x=middle, y=length))
        p= p+coord_polar(theta="x")
        p=p+ geom_tile(aes(width=middle_width,fill=polynomial,alpha=frequency))
        p=p+theme_bw()
        p=p+scale_x_continuous("middle point",limits=c(0,N),breaks = scales::pretty_breaks(n = 20))
        p=p+scale_y_continuous("length",limits=c(-0.5,NA),breaks = scales::pretty_breaks(n = 10))
        p=p+scale_alpha(limits=c(0,1),guide ="none",breaks = scales::pretty_breaks(n = 10))
        ncolors = length(unique(data$polynomial))
        maxcolors=brewer.pal.info["Set1","maxcolors"]

        tmp=unique(data[,c("type","polynomial")])
        tmp=tmp[order(as.numeric(tmp$type)),]
        labels=as.character(tmp$type)
        names(labels)=tmp$polynomial
        ###cut labels
        labels=ifelse(nchar(strtrim(labels,40))==nchar(labels),labels,paste(strtrim(labels,40),"..."))
        if(ncolors<=maxcolors)
            {
                p=p+scale_fill_brewer("",breaks=names(labels),labels=labels,palette = "Set1")
            }
        else
            {
                p=p+scale_fill_manual("",breaks=names(labels),labels=labels,values = colorRampPalette(brewer.pal(maxcolors, "Set1"))(ncolors))
            }
        if(flag_boundaries)
            {
                p=p+geom_segment(aes(x=middle1,y=length1,xend=middle2,yend=length2),alpha=1,color="white",size=0.5,data=data_boundaries)
            }

        if(!is.na(inputpath))
            {
                p=p+geom_tile(aes(width=middle_width),color="black",alpha=0,data=data_path_contours,size=0.2)
                if(!is.null(data_path_contours_missing))
                    {
                        p=p+geom_path(aes(group=group),color="black",data=data_path_contours_missing,size=0.2)
                    }
                p=p+geom_path(aes(group=group),color="blue",data=data_path_line,size=0.5,alpha=1)
                
            }        
        if(!is.na(inputpoint))
            {
                p=p+geom_point(color="yellow",data=data_point,size=3,alpha=1,shape=1)
            }
        print(p)
        dev.off()

    }
if(!flag_cyclic)
    {
        if(flag_boundaries)
            {
                data.mat=acast(unique(data[,c("start.index","end.index","polynomial")]),start.index~end.index,value.var="polynomial",fill=NA)
                data_boundaries=NULL
                ##difference start.index vs start.index+1
                ind=c(2:nrow(data.mat),1)
                tmp=data.frame(which(matrix(!is.na(data.mat)&!is.na(data.mat[ind,])&data.mat!=data.mat[ind,],nrow=nrow(data.mat),ncol=ncol(data.mat)),arr.ind=TRUE))
                tmp$start.index=as.numeric(rownames(data.mat)[tmp$row])
                tmp$end.index=as.numeric(colnames(data.mat)[tmp$col])
                tmp$start.index=tmp$start.index+0.5
                tmp$start.index1=tmp$start.index
                tmp$start.index2=tmp$start.index
                tmp$end.index1=tmp$end.index-0.5
                tmp$end.index2=tmp$end.index+0.5
                data_boundaries=unique(rbind(data_boundaries,tmp[,c("start.index1","start.index2","end.index1","end.index2")]))
                ##difference end.index vs end.index+1
                ind=c(2:ncol(data.mat),1)
                tmp=data.frame(which(matrix(!is.na(data.mat)&!is.na(data.mat[,ind])&data.mat!=data.mat[,ind],nrow=nrow(data.mat),ncol=ncol(data.mat)),arr.ind=TRUE))
                tmp$start.index=as.numeric(rownames(data.mat)[tmp$row])
                tmp$end.index=as.numeric(colnames(data.mat)[tmp$col])
                tmp$end.index=tmp$end.index+0.5
                tmp$start.index1=tmp$start.index-0.5
                tmp$start.index2=tmp$start.index+0.5
                tmp$end.index1=tmp$end.index
                tmp$end.index2=tmp$end.index
                data_boundaries=unique(rbind(data_boundaries,tmp[,c("start.index1","start.index2","end.index1","end.index2")]))
            }
        if(!is.na(inputpath))
            {
                data_path_line$group.end.index=cumsum(round(c(0,abs(data_path_line$end.index[-nrow(data_path_line)]-data_path_line$end.index[-1]))/(N/2)))/2
                data_path_line$group.start.index=cumsum(round(c(0,abs(data_path_line$start.index[-nrow(data_path_line)]-data_path_line[data_path_line$polynomial==data_path_line$polynomial[1],]$start.index[-1]))/(N/2)))/2
            }
        png(output,3000,2000,res=300)
        p=ggplot(data=data,aes(x=start.index, y=end.index))
        p=p+ geom_tile(aes(fill=polynomial,alpha=frequency))
        p=p+theme_bw()
        p=p+coord_fixed()
        p=p+xlab("start index")
        p=p+ylab("end index")
        p=p+scale_x_continuous(expand = c(0, 0),limits=c(-0.5,N+0.5),breaks = scales::pretty_breaks(n = 5))
        p=p+scale_y_reverse(expand = c(0, 0),limits=c(N+0.5,-0.5),breaks = scales::pretty_breaks(n = 5))
        p=p+scale_alpha(limits=c(0,1),guide ="none")
        ncolors = length(unique(data$polynomial))
        maxcolors=brewer.pal.info["Set1","maxcolors"]

        tmp=unique(data[,c("type","polynomial")])
        tmp=tmp[order(as.numeric(tmp$type)),]
        labels=as.character(tmp$type)
        names(labels)=tmp$polynomial
        ###cut labels
        labels=ifelse(nchar(strtrim(labels,40))==nchar(labels),labels,paste(strtrim(labels,40),"..."))
        if(ncolors<=maxcolors)
            {
                p=p+scale_fill_brewer("",breaks=names(labels),labels=labels,palette = "Set1")
            }
        else
            {
                p=p+scale_fill_manual("",breaks=names(labels),labels=labels,values = colorRampPalette(brewer.pal(maxcolors, "Set1"))(ncolors))
            }
        if(flag_boundaries)
            {
                p=p+geom_segment(aes(x=start.index1,y=end.index1,xend=start.index2,yend=end.index2),alpha=1,color="white",size=0.5,data=data_boundaries)
            }
        if(!is.na(inputpath))
            {
                p=p+geom_tile(color="black",alpha=0,data=data_path_contours)
                p=p+geom_path(aes(group=paste(group.start.index,group.end.index)),color="blue",data=data_path_line,size=0.5,alpha=1)
            }
        if(!is.na(inputpoint))
            {
                p=p+geom_point(color="yellow",data=data_point,size=3,alpha=1,shape=1)
            }
        print(p)
        dev.off()
    }

