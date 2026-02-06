# run bash <thisfilename.sh>
echo -n "Enter a comment: "
read a_comment
git add .
git commit -m "$a_comment"
git push origin main
