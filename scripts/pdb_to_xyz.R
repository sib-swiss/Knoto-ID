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
options(rgl.useNULL=TRUE)
library(Rpdb)

option_list = list(
  make_option(c("-o", "--output"), type = "character",
              help = "output filename (xyz format)."),
  make_option(c("-e", "--element-name"), type = "character", default = "CA",
              help = "select element with name ELEMENT-NAME."),
  make_option(c("-c", "--chain-id"), type = "character", default = NULL,
              help = "select chain with id CHAIN-ID. If not given, select first chain.")
    )

description="
   convert pdb file (FILENAME) to xyz format

   Examples:
      plot_knotted_core.R --output=3kzn.xyz 3kzn.pdb.gz 

    
"
opt_parser = OptionParser(usage = "usage: %prog [options] FILENAME",option_list=option_list, description = description)
opt = parse_args(opt_parser,positional_arguments=1,convert_hyphens_to_underscores = TRUE)

output = opt$options$output
element.name = opt$options$element_name
chain.id = opt$options$chain_id
inputfile = opt$arg
if(is.null(output))
{
    output=stdout()
}




data=read.pdb(inputfile)$atoms

if(is.null(chain.id))
{
    chain.id=data[1,"chainid"]
    if(length(unique(data$chainid))>1)
    {
     cat("******************************************************\n",file = stderr())
     cat("WARNING: input file contains multiple chain ids:\n",file = stderr())
     cat(unique(data$chainid),"\n",file = stderr())
     cat("Only chain with id",chain.id,"will be used.\n",file = stderr())
     cat("Please use option --chain-id to select another chain.\n",file = stderr())
     cat("******************************************************\n",file = stderr())
    }
}

#keep only chain.id and element.name
data=subset(data, recname=="ATOM"&chainid == chain.id&elename == element.name,reindex.all=FALSE)

##check gaps
gaps=data.frame(resid1=data[which(diff(data$resid)!=1),"resid"]+1,resid2=data[which(diff(data$resid)!=1)+1,"resid"]-1)
if(nrow(gaps)>0)
{
    cat("******************************************************\n",file = stderr())
    cat("WARNING: the selected chain is broken.\n",file = stderr())
    cat("Missing residue id:\n",file = stderr())
    cat(paste(" from ",apply(gaps,1,paste,collapse=" to "),collapse="\n"),"\n",file = stderr())
    cat("Knoto-ID will replace the missing parts with straight lines\n",file = stderr())
    cat("******************************************************\n",file = stderr())
}


write.table(data[,c("x1","x2","x3")],output,quote=FALSE,sep=" ",col.names=FALSE,row.names=FALSE)

