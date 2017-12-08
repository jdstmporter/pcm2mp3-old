use strict;
use warnings;
use v5.18;

sub namedValue {
	my $type = shift;
	
	<<"CLASS";
struct nv {
	const std::string name;
	const $type value;
	nv() : name(), value() {};
	nv(const std::string &n,const $type v) : name(n), value(v) {};
	virtual ~nv() = default;
	operator $type() const { return value; };
	operator std::string() const { return name; }; 
};
CLASS
}

sub makeEnum {
	my ($name,$terms) = @_;
	my @terms=@{$terms};
	push @terms, "Other";
	
	my $nv=namedValue("uint32_t");
	my $struct="${name}::nv";
	my $i=0;
	my @lines;
	my @defs;
	for(@terms) {
		@lines[$i]="static const ${struct} $_;";
		@defs[$i]="const ${struct} ${name}::$_ = ${struct}(\"$_\",$i);";
		$i++;
	}
	my $middle=join "\n", @lines;
	my $after=join "\n", @defs;
	my $all=join ",", @terms;
	
	<<"ENUM";
#include <string>
class $name {
	$nv
	$middle
	static ${struct} _all[];
};
$after
${struct} ${name}::_all[] = {$all};
ENUM
}

print makeEnum("Fred",["Astaire","Jones","Willard"]);

	
	
	
	
