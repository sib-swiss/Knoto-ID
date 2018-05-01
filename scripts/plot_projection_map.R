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
    make_option(c("-o", "--output"), type = "character", default = NULL,
                metavar="FILENAME",help = "output file (png format)."),
    make_option(c("--output-3D"), type = "character", default = NULL,
                metavar="FILENAME",help = "output 3D globe to file FILENAME (webGL)."),
    make_option(c("--curve-3D"), type = "character", default = NULL,
                metavar="FILENAME",help = "load piecewise linear curve in xyz format from file and add it to the 3D output. Only used with --output-3D.")
    )

description="
   Plot data contained in FILENAME in the form of a heatmap or as a
   3D globe (with --output-3D). Input file should be in the format
   produced by polynomial_invariant program using
   arguments --output-diagram=diagrams.txt

   Examples:
      plot_projection_map.R --output=output.png diagrams.txt
      plot_projection_map.R --output=output.png --output-3D=output.html diagrams.txt


    
"
opt_parser = OptionParser(usage = "usage: %prog [options] FILENAME",option_list=option_list, description = description)
opt = parse_args(opt_parser,positional_arguments=1,convert_hyphens_to_underscores = TRUE)

output = opt$options$output
output3D = opt$options$output_3D
curve3D = opt$options$curve_3D
inputmap = opt$arg

if(is.null(output)&is.null(output3D))
    {
        cat("ERROR: at least one output must be specified. Aborting...\n")
        quit(status=1)
    }

library(RColorBrewer) 
if(!is.null(output3D))
    {       
        options(rgl.useNULL=TRUE)
        library(rgl) #require pandoc
    }
library(ggplot2)
library(geometry)
library(geosphere)
library(rmarkdown)

#geosphere::distCosine(p1, p2, r=1) is unstable for small distances
distCosine=function(p1,p2){
    d=geosphere::distCosine(p1, p2, r=1)
    if(is.nan(d))
        {
            lon1=p1[1]*pi/180
            lat1=p1[2]*pi/180
            lon2=p2[1]*pi/180
            lat2=p2[2]*pi/180
            d=2*asin(sqrt(sin((lat2-lat1)/2)^2+cos(lat1)*cos(lat2)*sin((lon1-lon2)/2)^2))#more stable at short distances
        }
    return(d)
}


if(!is.null(curve3D))
    {
        cat("Loading  ",curve3D,"\n")
        curve=read.table(curve3D)
        colnames(curve)=c("x","y","z")
        ##rescale & recenter
        rangex=range(curve$x)
        rangey=range(curve$y)
        rangez=range(curve$z)
        scale=max(rangex[2]-rangex[1],rangey[2]-rangey[1],rangez[2]-rangez[1])
        centerx=(rangex[2]+rangex[1])/2
        centery=(rangey[2]+rangey[1])/2
        centerz=(rangez[2]+rangez[1])/2
        curve$x=1.9*(curve$x-centerx)/(scale)
        curve$y=1.9*(curve$y-centery)/(scale)
        curve$z=1.9*(curve$z-centerz)/(scale)
    }

cat("Loading  ",inputmap,"\n")

data=read.table(inputmap, sep="\t", header = T,comment.char="")
if(ncol(data)==6)
    {
        colnames(data)=c("x","y","z","type","polynomial","diagram")
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
        colnames(data)=c("x","y","z","polynomial","diagram")
        data$type=data$polynomial
        data$type=factor(data$type,levels=sort(unique(as.character(data$type))))
    }
data=unique(data)

##transform to longitude latitude (in degrees, since package geosphere use degree):
data$r=sqrt(data$x**2+data$y**2+data$z**2)
data$lon=atan2(data$y,data$x)*180/pi
data$lat=asin(data$z/data$r)*180/pi
##i.e.
##x=r*cos(lon*pi/180)*cos(lat*pi/180)
##y=r*sin(lon*pi/180)*cos(lat*pi/180)
##z=sin(lat*pi/180)


cat("Delaunay triangulation","\n")

##See http://www.qhull.org/html/qdelaun.htm
##"To compute the Delaunay triangulation of points on a sphere, compute their convex hull. If the sphere is the unit sphere at the origin, the facet normals are the Voronoi vertices of the input."
triangles.delaunay=convhulln(as.matrix(data[,c("x","y","z")]))
colnames(triangles.delaunay)=c("p1","p2","p3")
cat("Voronoi points","\n")
##delaunay triangle normals (=voronoi points)
triangles.delaunay=data.frame(cbind(triangles.delaunay,t(apply(triangles.delaunay,1,function(ind){    
    x1=data[ind["p1"],"x"]
    y1=data[ind["p1"],"y"]
    z1=data[ind["p1"],"z"]
    x2=data[ind["p2"],"x"]
    y2=data[ind["p2"],"y"]
    z2=data[ind["p2"],"z"]
    x3=data[ind["p3"],"x"]
    y3=data[ind["p3"],"y"]
    z3=data[ind["p3"],"z"]
    d13x=x3-x1
    d13y=y3-y1
    d13z=z3-z1
    d12x=x2-x1
    d12y=y2-y1
    d12z=z2-z1
    nx=d12y*d13z-d12z*d13y
    ny=d12z*d13x-d12x*d13z
    nz=d12x*d13y-d12y*d13x
    n=sqrt(nx^2+ny^2+nz^2)
    c(cx=(x1+x2+x3)/3,cy=(y1+y2+y3)/3,cz=(z1+z2+z3)/3,nx=nx/n,ny=ny/n,nz=nz/n)
}))))
##check orientation
barycenter=colMeans(data[,c("x","y","z")])
ind.to.reverse=sign((triangles.delaunay$cx-barycenter["x"])*triangles.delaunay$nx+
    (triangles.delaunay$cy-barycenter["y"])*triangles.delaunay$ny+
    (triangles.delaunay$cz-barycenter["z"])*triangles.delaunay$nz)<0
triangles.delaunay[ind.to.reverse,c("p1","p2","p3")]=triangles.delaunay[ind.to.reverse,c("p1","p3","p2")]
triangles.delaunay[ind.to.reverse,c("nx","ny","nz")]=-triangles.delaunay[ind.to.reverse,c("nx","ny","nz")]

triangles.delaunay$lat=asin(triangles.delaunay$nz)*180/pi
triangles.delaunay$lon=atan2(triangles.delaunay$ny,triangles.delaunay$nx)*180/pi
triangles.delaunay[abs(triangles.delaunay$nz)==1,"lon"]=0

triangles.delaunay$ID=1:nrow(triangles.delaunay)

##voronoi facets
cat("Voronoi facets","\n")
voronoi.facets=unique(rbind(
    data.frame(p=triangles.delaunay$p1,t=triangles.delaunay$ID),
    data.frame(p=triangles.delaunay$p2,t=triangles.delaunay$ID),
    data.frame(p=triangles.delaunay$p3,t=triangles.delaunay$ID)
    ))

voronoi.facets=cbind(voronoi.facets,lon1=data[voronoi.facets$p,c("lon")],lat1=data[voronoi.facets$p,c("lat")],lon2=triangles.delaunay[voronoi.facets$t,c("lon")],lat2=triangles.delaunay[voronoi.facets$t,c("lat")])
##remove duplicate (due to coplanar points)
voronoi.facets=voronoi.facets[!duplicated(voronoi.facets[,c("p","lon1","lat1","lon2","lat2")]),]
voronoi.facets$angle=apply(voronoi.facets,1,function(x){
    bearing(x[c("lon1","lat1")],x[c("lon2","lat2")], a=1, f=0)
})

voronoi.facets=do.call(rbind,by(voronoi.facets[,c("p","t","angle")],voronoi.facets$p,function(x){
        ind=order(x$angle)
        tmp=data.frame(p=x[ind,"p"],t1=x[ind,"t"],t2=x[c(ind[-1],ind[1]),"t"])

        return(tmp)
    }))

  
voronoi.facets=cbind(voronoi.facets,lon1=triangles.delaunay[voronoi.facets[,"t1"],"lon"],lon2=triangles.delaunay[voronoi.facets[,"t2"],"lon"],lat1=triangles.delaunay[voronoi.facets[,"t1"],"lat"],lat2=triangles.delaunay[voronoi.facets[,"t2"],"lat"])


voronoi.facets=voronoi.facets[,c("p","lon1","lon2","lat1","lat2")]
voronoi.facets$group=as.character(voronoi.facets$p)

##split edges crossing boundary (phi=-180 or phi=180)
cat("Split edges","\n")
across.boundary=voronoi.facets$p[(abs(voronoi.facets$lon1-voronoi.facets$lon2) > 180 )]
voronoi.facets.tosplit=voronoi.facets[voronoi.facets$p%in%across.boundary,]
voronoi.facets.tosplit=do.call(rbind,by(voronoi.facets.tosplit,voronoi.facets.tosplit$p,function(x){    
    crossing=sign(ifelse(abs(x$lon1 - x$lon2)>180,x$lon1 - x$lon2,0))
    ind.crossings=which(crossing!=0)
    if(length(ind.crossings)==2)
        {
            side=cumsum(crossing)
            x$group=paste(x$p,(side)%%2+1,sep="_")

            if(distCosine(x[ind.crossings[1],c("lon1","lat1")],x[ind.crossings[1],c("lon2","lat2")])>0)
                {
                    p1=matrix(gcIntersect(x[ind.crossings[1],c("lon1","lat1")],x[ind.crossings[1],c("lon2","lat2")],c(180,0),c(180,90)),ncol=2,byrow=TRUE)
                    p1=p1[abs(p1[,1]-180)<1e-10|abs(p1[,1]+180)<1e-10,,drop=FALSE]
                }
            else
                {
                    p1=x[ind.crossings[1],c("lon1","lat1")]
                }
            l1=x[ind.crossings[1],]
            l1$lon2=ifelse(sign(l1$lon1)!=0,sign(l1$lon1)*180,-sign(l1$lon2)*180)
            l1$lat2=p1[2]
            l1$group=x[ind.crossings[2],"group"]
            x[ind.crossings[1],"lon1"]=sign(x[ind.crossings[1],"lon2"])*180
            x[ind.crossings[1],"lat1"]=p1[2]

            if(distCosine(x[ind.crossings[2],c("lon1","lat1")],x[ind.crossings[2],c("lon2","lat2")])>0)
                {
                    p2=matrix(gcIntersect(x[ind.crossings[2],c("lon1","lat1")],x[ind.crossings[2],c("lon2","lat2")],c(180,0),c(180,90)),ncol=2,byrow=TRUE)
                    p2=p2[abs(p2[,1]-180)<1e-10|abs(p2[,1]+180)<1e-10,,drop=FALSE]
                }
            else
                {
                    p2=x[ind.crossings[2],c("lon1","lat1")]
                }
            
            l2=x[ind.crossings[2],]
            l2$lon2=ifelse(sign(l2$lon1)!=0,sign(l2$lon1)*180,-sign(l2$lon2)*180)
            l2$lat2=p2[2]
            l2$group=x[ind.crossings[1],"group"]

            x[ind.crossings[2],"lon1"]=sign(x[ind.crossings[2],"lon2"])*180
            x[ind.crossings[2],"lat1"]=p2[2]
            ind1=(ind.crossings[1]):(ind.crossings[2]-1)
            ind2=(ind.crossings[2]):nrow(x)
            if(ind.crossings[1]>1)
                ind2=c(ind2,1:(ind.crossings[1]-1))
            rbind(x[ind1,],l2,x[ind2,],l1)
        }
    else if(length(ind.crossings)==1)#region containing a pole 
        {
            pole.lat=ifelse(x[ind.crossings[1],"lon2"]-x[ind.crossings[1],"lon1"]<0,-90,90)
            p1=matrix(gcIntersect(x[ind.crossings[1],c("lon1","lat1")],x[ind.crossings[1],c("lon2","lat2")],c(180,0),c(180,90)),ncol=2,byrow=TRUE)
            p1=p1[abs(p1[,1]-180)<1e-10|abs(p1[,1]+180)<1e-10,,drop=FALSE]
            l1=x[ind.crossings[1],]
            l1$lon2=sign(l1$lon1)*180
            l1$lat2=p1[2]
            x[ind.crossings[1],"lon1"]=sign(x[ind.crossings[1],"lon2"])*180
            x[ind.crossings[1],"lat1"]=p1[2]
            closure1=l1
            closure1$lon1=closure1$lon2
            closure1$lat1=closure1$lat2
            closure1$lat2=pole.lat
            closure2=x[ind.crossings[1],]
            closure2$lon2=closure2$lon1
            closure2$lat2=closure2$lat1
            closure2$lat1=pole.lat
            ind=ind.crossings[1]:nrow(x)
            if(ind.crossings[1]>1)
                ind=c(ind,1:(ind.crossings[1]-1))
            rbind(closure2,x[ind,],l1,closure1)
        }
    }))

voronoi.facets=rbind(voronoi.facets[!voronoi.facets$p%in%across.boundary,],voronoi.facets.tosplit)

voronoi.facets$group=as.numeric(factor(voronoi.facets$group))


cat("Great circles","\n")
tmp=apply(voronoi.facets,1,function(x){
        d=distCosine(x[c("lon1","lat1")],x[c("lon2","lat2")])
        if(d>(pi/50)/2)
            {
                cbind(p=x[["p"]],group=x[["group"]],(gcIntermediate(x[c("lon1","lat1")],x[c("lon2","lat2")],n=round((1-cos(max(abs(x[c("lat1","lat2")]))*pi/180))*d/(pi/50)),breakAtDateLine=FALSE, addStartEnd=TRUE)))
            }
        else
            {
                cbind(p=x[["p"]],group=x[["group"]],rbind(x[c("lon1","lat1")],x[c("lon2","lat2")]))
            }
        
    })
tmp=do.call(rbind,tmp)
tmp=data.frame(tmp,row.names=1:nrow(tmp))
colnames(tmp)=gsub("^lon[12]$","lon",gsub("^lat[12]$","lat",colnames(tmp)))
voronoi.facets=tmp


voronoi.facets$polynomial=data[voronoi.facets$p,"polynomial"]

cat("Preparing plot","\n")
p=ggplot(aes(x=lon,y=lat),data=cbind(data,ID=1:nrow(data)))
p=p+geom_polygon(aes(group=group,fill=polynomial),color="white",size=0.01,data=voronoi.facets) 
p=p+scale_x_continuous(expand = c(0, 0),breaks =seq(-180,180,60))
p=p+scale_y_continuous(expand = c(0, 0),breaks =seq(-90,90,30))
p=p+xlab("longitude")
p=p+ylab("latitude")
p=p+coord_fixed(xlim=c(-180,180),ylim=c(-90,90))
p=p+theme_bw()
ncolors = length(unique(data$polynomial))
maxcolors=brewer.pal.info["Set1","maxcolors"]
{
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
}
if(!is.null(output))
    {

        cat("creating  ",output,"\n")
        png(output,3000,2000,res=300)
        print(p)
        dev.off()
    }
if(!is.null(output3D))
    {       
        tmpfile = tempfile()

        cat("creating temporaty file ",tmpfile,"\n")
        png(tmpfile,3000,1500,res=300)
        p=p+labs(x = NULL, y = NULL, title=NULL)
        p=p+theme(panel.grid=element_blank())
        p=p+theme(panel.border=element_blank())
        p=p+theme(plot.margin=unit(c(0,0,0,0), "null"))
        p=p+theme(panel.spacing=unit(0, "null"))
        p=p+theme(axis.ticks=element_blank())
        p=p+theme(axis.text=element_blank())
        p=p+theme(axis.title=element_blank())
        p=p+theme(axis.line=element_blank())
        p=p+theme(legend.position="none")
        p=p+theme(axis.ticks.length=unit(0, "null"))
        p=p+theme(legend.margin=margin(0,0,0,0))        
        print(p)
        dev.off()

        if(!is.null(curve3D))
            {
                mfrow3d(1, 2, sharedMouse = TRUE)
            }
        view3d(phi=-90,fov=0)#orthographic projection
        r=1
        N=30
        lat = matrix(seq(pi/2, -pi/2, len = N), N, N, byrow = TRUE)
        lon = matrix(seq(-pi, pi, len = N), N, N)
        x = r*cos(lat)*cos(lon)
        y = r*cos(lat)*sin(lon)
        z = r*sin(lat)
        persp3d(x, y, z, col = "white",texture = tmpfile,specular = "black", axes = FALSE, box = FALSE, xlab = "", ylab = "", zlab = "", normal_x = x, normal_y = y, normal_z = z)
        rgl.lines(c(-1.5, 1.5), c(0, 0), c(0, 0), color = "black")
        rgl.lines(c(0, 0), c(-1.5,1.5), c(0, 0), color = "black")
        rgl.lines(c(0, 0), c(0, 0), c(-1.5,1.5), color = "black")
        
        rgl.texts(1.5,0,0, text = "x", color = "black", adj = c(0.5, -0.8),cex=2)
        rgl.texts(0,1.5,0, text = "y", color = "black", adj = c(0.5, -0.8),cex=2)
        rgl.texts(0,0,1.5, text = "z", color = "black", adj = c(0.5, -0.8),cex=2)
        if(!is.null(curve3D))
            {
                next3d()

                ##evaluate tangent
                e1=curve
                e1=e1[-1,]-e1[-nrow(e1),]
                ##duplicate points
                curve=rbind(curve[1,],curve[rep(2:(nrow(curve)-1),each=2),] ,curve[nrow(curve),])
                e1=e1[rep(1:nrow(e1),each=2),]
                ##perturb duplicate points (otherwise cylinder3d fails)
                curve[seq(2,nrow(curve)-1,by=2),]=curve[seq(2,nrow(curve)-1,by=2),]-0.001*e1[seq(2,nrow(curve)-1,by=2),]
                view3d(phi=-90,fov=0)#orthographic projection                
                shade3d(cylinder3d(curve,radius=0.02,sides=12,closed=0,e1=e1), col = "red")
                rgl.lines(c(-1.5, 1.5), c(0, 0), c(0, 0), color = "black")
                rgl.lines(c(0, 0), c(-1.5,1.5), c(0, 0), color = "black")
                rgl.lines(c(0, 0), c(0, 0), c(-1.5,1.5), color = "black")
                
                rgl.texts(1.5,0,0, text = "x", color = "black", adj = c(0.5, -0.8),cex=2)
                rgl.texts(0,1.5,0, text = "y", color = "black", adj = c(0.5, -0.8),cex=2)
                rgl.texts(0,0,1.5, text = "z", color = "black", adj = c(0.5, -0.8),cex=2)
            }

        
        filename=output3D 
        cat("creating  ",filename,"\n")
        ##NOTE: selfcontained=TRUE -> require pandoc.
        if(pandoc_available())
            {
               htmlwidgets::saveWidget(rglwidget(sizingPolicy=htmlwidgets::sizingPolicy(padding = 0, browser.fill = TRUE,viewer.fill=TRUE)), normalizePath(filename,mustWork =TRUE))
            }
        else
            {
               cat("***************************************************\n")
               cat("WARNING: could not create self contained html file\n")
               cat(filename,"\n")               
               cat("External resources are saved in directory\n")
               cat(paste(tools::file_path_sans_ext(basename(filename)),"_files", sep = ""),"/ \n",sep="")
               cat("To avoid this message, install pandoc(http://pandoc.org/)\n")
               cat("and make sure it can be found by R.\n")
               cat("***************************************************\n")
               htmlwidgets::saveWidget(rglwidget(sizingPolicy=htmlwidgets::sizingPolicy(padding = 0, browser.fill = TRUE,viewer.fill=TRUE)), normalizePath(filename,mustWork =TRUE),selfcontained=FALSE)
            }
    }

