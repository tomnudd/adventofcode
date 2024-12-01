my @a1;
my @a2;
my %c;
open my $in,"<","input.txt";
while(<$in>)
{
    chomp;
    /(\d+)   (\d+)/;
    push @a1,$1;
    push @a2,$2;
    $c{$2}++;
}
close $in;
@a1=sort @a1;
@a2=sort @a2;

my $sum = 0;
$sum += abs($a1[$_]-$a2[$_]) for (0..$#a1);
print "$sum\n";

$sum = 0;
$sum += $a1[$_]*($c{$a1[$_]}) for (0..$#a1);
print "$sum\n";